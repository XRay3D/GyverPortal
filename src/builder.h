#pragma once

// GP Builder

// #include "buildMacro.h"
#include "canvas.h"
#include "log.h"
#include "objects.h"
#include "scripts.h"
#include "utils.h"
#include "version.h"
#include <FS.h>

#ifdef ESP8266
#include <ESP8266WebServer.h>
#else
#include <WebServer.h>
#endif

namespace GP {

extern WebServer__* __server__;

extern int _gp_bufsize;
extern String* _gp_uri;
extern String* tmpPageBuf;
extern uint32_t _gp_unix_tmr;
extern uint32_t _gp_local_unix;
extern const char* _gp_style;
extern uint8_t _gp_seed;
extern const char* _gp_mdns;

struct Builder {
    uint8_t _gp_nav_pos = 0;
    uint8_t _gp_nav_id = 0;
    int _spinInt = 200;

    // период изменения значения при удержании кнопки спиннера
    void setSpinnerPeriod(int prd) {
        _spinInt = prd;
    }

    // установить таймаут ожидания отправки килков и апдейтов
    void setTimeout(int tout) {
        JS_BEGIN();
        *tmpPageBuf += F("_tout=");
        *tmpPageBuf += tout;
        JS_END();
    }

    // ======================= БИЛДЕР =======================
    void BUILD_BEGIN(int width = 350) {
        PAGE_BEGIN();
        JS_TOP();
        PAGE_BLOCK_BEGIN(width);
    }
    void BUILD_BEGIN(PGM_P style, int width = 350) {
        PAGE_BEGIN();
        THEME(style);
        JS_TOP();
        PAGE_BLOCK_BEGIN(width);
    }

    void BUILD_BEGIN_FILE(int width = 350) {
        PAGE_BEGIN();
        JS_TOP_FILE();
        PAGE_BLOCK_BEGIN(width);
    }
    void BUILD_BEGIN_FILE(const String& style, int width = 350) {
        PAGE_BEGIN();
        THEME_FILE(style);
        JS_TOP_FILE();
        PAGE_BLOCK_BEGIN(width);
    }

    void BUILD_END() {
        PAGE_BLOCK_END();
        JS_BOTTOM();
        PAGE_END();
    }

    void PAGE_TITLE(const String& text = "", const String& name = "") {
        if (name.length()) HIDDEN(name, F("_title"), text);
        if (text.length()) {
            JS_BEGIN();
            *tmpPageBuf += F("document.title='");
            *tmpPageBuf += text;
            *tmpPageBuf += "'";
            JS_END();
        }
    }

    // ===================== ОТПРАВКА RAW =====================
    void SEND(const String& s) {
        *tmpPageBuf += s;
        send();
    }
    void SEND_P(PGM_P s) {
        send(true);
        __server__->sendContent_P(s);
    }

    void send(bool force = 0) {
        if ((int)tmpPageBuf->length() > (force ? 0 : _gp_bufsize)) {
            __server__->sendContent(*tmpPageBuf);
            *tmpPageBuf = "";
        }
    }

    // ========================== UI БЛОК ==========================
    PGM_P _ui_style = GREEN;

    void UI_BEGIN(const String& title, const String& urls, const String& names, PGM_P st = GREEN, int w = 1000) {
        UI_MENU(title, st);
        Parser n(names);
        Parser u(urls);
        while (n.parse()) {
            u.parse();
            UI_LINK(u.str, n.str);
        }
        UI_BODY(w);
    }

    void UI_MENU(const String& title, PGM_P st = GREEN) {
        _ui_style = st;
        *tmpPageBuf += F("<style>.mainblock{max-width:100%!important}</style>\n"); // force full width
        *tmpPageBuf += F("<div class='headbar'><div class='burgbtn' id='menuToggle' onclick='sdbTgl()'><span></span><span></span><span></span></div>\n<div class='header'>");
        *tmpPageBuf += title;
        *tmpPageBuf += F("</div></div>\n<nav class='sidebar' id='dashSdb'><div class='sblock'><div class='header header_s'>");
        *tmpPageBuf += title;
        *tmpPageBuf += F("</div>\n");
        send();
    }
    void UI_BODY(int w = 1000) {
        *tmpPageBuf += F("</div></nav>\n<div class='overlay' onclick='sdbTgl()' id='dashOver'></div><div class='page'><div class='ui_block'");
        if (w != 1000) {
            *tmpPageBuf += F(" style='max-width:");
            *tmpPageBuf += w;
            *tmpPageBuf += F("px'");
        }
        *tmpPageBuf += ">\n";
        send();
    }
    void UI_END() {
        SEND(F("</div></div>\n"));
    }

    void UI_LINK(const String& url, const String& name) {
        *tmpPageBuf += F("<a href='");
        *tmpPageBuf += url;
        *tmpPageBuf += "'";
        if (_gp_uri->equals(url)) {
            *tmpPageBuf += F(" class='sbsel' style='background:");
            *tmpPageBuf += FPSTR(_ui_style);
            *tmpPageBuf += F(" !important;'");
        }
        *tmpPageBuf += ">";
        *tmpPageBuf += name;
        *tmpPageBuf += F("</a>\n");
    }

    // ======================= СТРАНИЦА =======================
    void PAGE_BEGIN() {
        _gp_nav_id = 0;
        SEND(F("<!DOCTYPE HTML><html><head>\n"
               "<meta charset='utf-8'>\n"
               "<meta name='viewport' content='width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=0'>\n"
               "<meta name='apple-mobile-web-app-capable' content='yes'/>\n"
               "<meta name='mobile-web-app-capable' content='yes'/>\n"
               "</head><body>\n"));
    }
    void JS_TOP_FILE() {
        SEND(F("<script src='/gp_data/scripts.js?=" GP_VERSION "'></script>\n"));
        updateTime();
    }
    void JS_TOP() {
        *tmpPageBuf += F("<script src='/GP_SCRIPT.js?v" GP_VERSION "=");
        *tmpPageBuf += _gp_seed;
        *tmpPageBuf += F("'></script>\n");
        updateTime();
    }
    void JS_BOTTOM() {
        SEND(F("<script>document.querySelectorAll('input[type=range]').forEach(x=>{GP_change(x)});\n"
               "document.querySelectorAll('.spin_inp').forEach(x=>GP_spinw(x));\n"
               "</script>\n"));
    }

    // время
    uint32_t _timeUpdPrd = 10 * 60 * 1000ul;
    void setTimeUpdatePeriod(uint32_t t) { _timeUpdPrd = t; }
    void updateTime() {
        if (!_gp_unix_tmr || millis() - _gp_unix_tmr >= _timeUpdPrd)
            SEND(F("<script>GP_send('/GP_time?GPunix='+Math.round(new Date().getTime()/1000)+'&gmt='+(-new Date().getTimezoneOffset()));</script>\n"));
    }

    void THEME(PGM_P style) {
        *tmpPageBuf += F("<link rel='stylesheet' href='/GP_STYLE.css?v" GP_VERSION "=");
        *tmpPageBuf += ((unsigned long)style) & 0xFFFF;
        *tmpPageBuf += "'";
        *tmpPageBuf += ">\n";
        _gp_style = style;
    }
    void THEME_FILE(const String& style) {
        *tmpPageBuf += F("<link rel='stylesheet' href='/gp_data/");
        *tmpPageBuf += style;
        *tmpPageBuf += F(".css?=" GP_VERSION "'>\n");
        send();
    }

    void PAGE_BLOCK_BEGIN(int width = 350) {
        *tmpPageBuf += F("<div align='center' class='mainblock'");
        if (width) {
            *tmpPageBuf += F(" style='max-width:");
            *tmpPageBuf += width;
            *tmpPageBuf += F("px'");
        }
        *tmpPageBuf += ">\n";
        send();
    }
    void PAGE_BLOCK_END() {
        SEND(F("</div>\n<div id='onlBlock' class='onlBlock'>🚫 Device offline! 🚫</div>\n"));
    }
    void PAGE_END() {
        SEND(F("</body></html>"));
    }

    void SPOILER_BEGIN(const String& text, PGM_P st = GREEN) {
        *tmpPageBuf += F("<details><summary align='left' style='");
        if (st != GREEN) {
            *tmpPageBuf += F("background-color:");
            *tmpPageBuf += FPSTR(st);
            *tmpPageBuf += ';';
        }
        *tmpPageBuf += F("'>");
        *tmpPageBuf += text;
        *tmpPageBuf += F("</summary><div align='center' style='");
        if (st != GREEN) {
            *tmpPageBuf += F("border-color:");
            *tmpPageBuf += FPSTR(st);
            *tmpPageBuf += ';';
        }
        *tmpPageBuf += F("'>\n");
        send();
    }
    void SPOILER_END() {
        SEND(F("</div></details>\n"));
    }

    void HINT(const String& name, const String& txt) {
        JS_BEGIN();
        *tmpPageBuf += F("GP_hint('");
        *tmpPageBuf += name;
        *tmpPageBuf += F("','");
        *tmpPageBuf += txt;
        *tmpPageBuf += F("')");
        JS_END();
        send();
    }

    void JS_BEGIN() {
        *tmpPageBuf += F("<script>\n");
    }
    void JS_END() {
        *tmpPageBuf += F("\n</script>\n");
    }

    void ONLINE_CHECK(int prd = 3000) {
        JS_BEGIN();
        *tmpPageBuf += F("setInterval(function(){if(!document.hidden){var xhttp=new XMLHttpRequest();xhttp.timeout=");
        *tmpPageBuf += prd;
        *tmpPageBuf += F(";xhttp.open('GET','/GP_ping?',true);xhttp.send();\n"
                         "xhttp.onreadystatechange=function(){onlShow(!this.status)}}},");
        *tmpPageBuf += prd;
        *tmpPageBuf += F(");\n");
        JS_END();
    }

    // ================== CANVAS ==================
    // обновить CANVAS с именем name при клике на компонент из списка list
    void REDRAW_CLICK(const String& name, const String& list) {
        JS_BEGIN();
        *tmpPageBuf += F("_clkRedrList['");
        *tmpPageBuf += name;
        *tmpPageBuf += F("']='");
        *tmpPageBuf += list;
        *tmpPageBuf += F("';");
        JS_END();
        send();
    }
    void CANVAS(const String& name, int w, int h) {
        *tmpPageBuf += F("<canvas class='_canvas' id='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' width='");
        *tmpPageBuf += w;
        *tmpPageBuf += F("' height='");
        *tmpPageBuf += h;
        *tmpPageBuf += F("'></canvas>\n");
        send();
    }

    void CANVAS_SUPPORT() {
        *tmpPageBuf += F("<script src='/GP_CANVAS.js?=");
        *tmpPageBuf += _gp_seed;
        *tmpPageBuf += F("'></script>\n");
    }

    void CANVAS_SUPPORT_FILE() {
        SEND(F("<script src='/gp_data/canvas.js?=" GP_VERSION "'></script>\n"));
    }

    void CANVAS_BEGIN(const String& name, int w, int h) {
        CANVAS(name, w, h);
        JS_BEGIN();
        *tmpPageBuf += F("var cv=getEl('");
        *tmpPageBuf += name;
        *tmpPageBuf += F("');\n var cx=cv.getContext('2d');eval(GP::canvas(\"");
    }
    void CANVAS_END() {
        *tmpPageBuf += F("\"))");
        JS_END();
    }

    // ======================= UPDATE =======================
    void UPDATE(const String& list, int prd = 1000) {
        JS_BEGIN();
        *tmpPageBuf += F("setInterval(function(){if(!document.hidden)GP_update('");
        *tmpPageBuf += list;
        *tmpPageBuf += F("')},");
        *tmpPageBuf += prd;
        *tmpPageBuf += F(");");
        JS_END();
        send();
    }

    void JQ_SUPPORT_FILE() {
        SEND(F("<script src='/gp_data/jquery.js'></script>\n"));
    }
    void JQ_SUPPORT() {
        SEND(F("<script src='https://code.jquery.com/jquery-3.6.1.min.js'></script>\n"));
    }
    void JQ_UPDATE_BEGIN(int prd = 1000, int del = 100) {
        JS_BEGIN();
        *tmpPageBuf += F("var globalupd=1;\n"
                         "$(function(ev){$('#jqupd').click(function (ev){globalupd=0;setTimeout(function(){$('#jqupd').load('# #jqupd');},");
        *tmpPageBuf += del;
        *tmpPageBuf += F(");});});\n setInterval(function(){if(globalupd){$('#jqupd').load('# #jqupd');}else{globalupd=1;}},");
        *tmpPageBuf += prd;
        *tmpPageBuf += F(");");
        JS_END();
        *tmpPageBuf += F("<div id='jqupd' style='width:100%;'>\n");
        send();
    }
    void JQ_UPDATE_END() {
        SEND(F("</div>\n"));
    }

    // перезагрузка. Имя нужно также указать в списке update
    void RELOAD(const String& name) {
        HIDDEN(name, F("_reload"), "");
    }

    // список имён компонентов, изменение (клик) которых приведёт к перезагрузке страницы
    void RELOAD_CLICK(const String& list) {
        JS_BEGIN();
        *tmpPageBuf += F("_clkRelList='");
        *tmpPageBuf += list;
        *tmpPageBuf += F("'.split(',');");
        JS_END();
        send();
    }

    // вызвать update у компонентов в списке list при клике по компонентам из списка names
    void UPDATE_CLICK(const String& list, const String& names) {
        JS_BEGIN();
        *tmpPageBuf += F("_clkUpdList['");
        *tmpPageBuf += names;
        *tmpPageBuf += F("']='");
        *tmpPageBuf += list;
        *tmpPageBuf += F("';");
        JS_END();
        send();
    }

    // выполнить код по ответу на обновление
    void EVAL(const String& name, const String& code = "") {
        HIDDEN(name, F("_eval"), code);
    }

    // ====================== ТАБЛИЦЫ ======================
    Align* _als = nullptr;
    int _alsCount = 0;

    void TABLE_BORDER(bool show) {
        *tmpPageBuf += F("<style>td{border:");
        *tmpPageBuf += show ? F("1px solid") : F("none");
        *tmpPageBuf += F("}</style>\n");
    }

    void TABLE_BEGIN(const String& tdw = "", Align* als = nullptr, const String& w = "100%") {
        _als = als;
        *tmpPageBuf += F("<table width='");
        *tmpPageBuf += w;
        *tmpPageBuf += F("'>\n");
        send();

        if (tdw.length()) {
            // TR();
            *tmpPageBuf += F("<tr style='visibility:collapse;'>\n");
            Parser p(tdw);
            while (p.parse()) {
                if (p.str.length()) {
                    *tmpPageBuf += F("<td width='");
                    *tmpPageBuf += p.str;
                    *tmpPageBuf += F("'>\n");
                }
            }
        }
    }
    void TR(Align al = Align::CENTER) {
        _alsCount = 0;
        *tmpPageBuf += F("<tr align='");
        *tmpPageBuf += FPSTR(getAlign(al));
        *tmpPageBuf += F("'>\n");
    }
    void TD(Align al = Align::CENTER, uint8_t cs = 1, uint8_t rs = 1) {
        *tmpPageBuf += F("<td");
        if (al != Align::CENTER || _als) {
            *tmpPageBuf += F(" align=");
            if (al == Align::CENTER && _als && _als[_alsCount] >= Align::CENTER && _als[_alsCount] <= Align::JUSTIFY) *tmpPageBuf += FPSTR(getAlign(_als[_alsCount++]));
            else *tmpPageBuf += FPSTR(getAlign(al));
        }
        if (cs > 1) {
            *tmpPageBuf += F(" colspan=");
            *tmpPageBuf += cs;
        }
        if (rs > 1) {
            *tmpPageBuf += F(" rowspan=");
            *tmpPageBuf += rs;
        }
        *tmpPageBuf += ">\n";
        send();
    }
    void TABLE_END() {
        _als = nullptr;
        SEND(F("</table>\n"));
    }

    // ====================== ПОПАПЫ =======================
    void ALERT(const String& name) {
        HIDDEN(name, F("_alert"), "");
        send();
    }
    void ALERT(const String& name, const String& text) {
        HIDDEN(name, F("_alert"), text);
        send();
    }

    // отправит null если нажать отмена
    void PROMPT(const String& name) {
        HIDDEN(name, F("_prompt"), "");
        send();
    }
    void PROMPT(const String& name, const String& text) {
        HIDDEN(name, F("_prompt"), text);
        send();
    }

    void CONFIRM(const String& name) {
        HIDDEN(name, F("_confirm"), "");
        send();
    }
    void CONFIRM(const String& name, const String& text) {
        HIDDEN(name, F("_confirm"), text);
        send();
    }

    // ======================= ФОРМА =======================
    void FORM_BEGIN(const String& name) {
        *tmpPageBuf += F("<form action='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' id='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' method='POST'>\n<input type='hidden' name='GP_form' value='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("'>\n");
        send();
    }
    void FORM_END() {
        SEND(F("</form>\n"));
    }
    void SUBMIT(const String& text, PGM_P st = GREEN, const String& cls = "") {
        *tmpPageBuf += F("<input type='submit' value='");
        *tmpPageBuf += text;
        if (st != GREEN) {
            *tmpPageBuf += F("' style='background:");
            *tmpPageBuf += FPSTR(st);
        }
        if (cls.length()) {
            *tmpPageBuf += F("' class='");
            *tmpPageBuf += cls;
        }
        *tmpPageBuf += F("'>\n");
        send();
    }
    void SUBMIT_MINI(const String& text, PGM_P st = GREEN) {
        SUBMIT(text, st, F("miniButton"));
    }

    void FORM_SEND(const String& text, const String& url = "", PGM_P st = GREEN, const String& cls = "") {
        *tmpPageBuf += F("<input type='button' onclick='GP_sendForm(this.parentNode.id,\"");
        *tmpPageBuf += url;
        *tmpPageBuf += F("\")' value='");
        *tmpPageBuf += text;
        if (st != GREEN) {
            *tmpPageBuf += F("' style='background:");
            *tmpPageBuf += FPSTR(st);
        }
        if (cls.length()) {
            *tmpPageBuf += F("' class='");
            *tmpPageBuf += cls;
        }
        *tmpPageBuf += F("'>\n");
        send();
    }
    void FORM_SEND_MINI(const String& text, const String& url = "", PGM_P st = GREEN) {
        FORM_SEND(text, url, st, F("miniButton"));
    }

    void FORM_SUBMIT(const String& name, const String& text, PGM_P st = GREEN) {
        FORM_BEGIN(name);
        SUBMIT(text, st);
        FORM_END();
    }
    void HIDDEN(const String& name, const String& value) {
        *tmpPageBuf += F("<input type='hidden' name='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' value='");
        *tmpPageBuf += value;
        *tmpPageBuf += F("' id='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("'>\n");
        send();
    }
    void HIDDEN(const String& id, const String& name, const String& value) {
        *tmpPageBuf += F("<input type='hidden' name='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' value=\"");
        *tmpPageBuf += value;
        *tmpPageBuf += F("\" id='");
        *tmpPageBuf += id;
        *tmpPageBuf += F("'>\n");
        send();
    }
    void FORM_SUBMIT(const String& name, const String& text, const String& namehidden, const String& valuehidden, PGM_P st = GREEN) {
        FORM_BEGIN(name);
        HIDDEN(namehidden, valuehidden);
        SUBMIT(text, st);
        FORM_END();
    }

    // ======================= ОФОРМЛЕНИЕ =======================
    void GRID_BEGIN(int width = 0) {
        *tmpPageBuf += F("<div class='grid' id='grid'");
        if (width) {
            *tmpPageBuf += F(" style='max-width:");
            *tmpPageBuf += width;
            *tmpPageBuf += F("px'");
        }
        *tmpPageBuf += ">\n";
        send();
    }
    void GRID_END() {
        BLOCK_END();
    }
    void GRID_RESPONSIVE(int width) {
        *tmpPageBuf += F("<style type='text/css'>@media screen and (max-width:");
        *tmpPageBuf += width;
        *tmpPageBuf += F("px){\n.grid{display:block;}\n#grid .block{margin:20px 5px;width:unset;}}</style>\n");
        send();
    }

    void BLOCK_BEGIN(Block type, const String& width = "", const String& text = "", PGM_P st = DEFAULT) {
        *tmpPageBuf += F("<div class='");
        if (type != Block::DIV_RAW) *tmpPageBuf += F("blockBase");
        if (type != Block::DIV && type != Block::DIV_RAW) {
            *tmpPageBuf += F(" block");
            if (text.length()) *tmpPageBuf += F(" blockTab");
            if (type == Block::THIN) *tmpPageBuf += F(" thinBlock");
        }
        *tmpPageBuf += "'";
        if (type == Block::TAB) *tmpPageBuf += F(" id='blockBack'");

        if (width.length()) {
            *tmpPageBuf += F(" style='max-width:");
            *tmpPageBuf += width;
            *tmpPageBuf += "'";
        }
        if (type == Block::THIN && st != DEFAULT) {
            *tmpPageBuf += F(" style='border:2px solid");
            *tmpPageBuf += FPSTR(st);
            *tmpPageBuf += "'";
        }
        *tmpPageBuf += ">\n";

        if (text.length()) {
            if (type == Block::DIV || type == Block::DIV_RAW) {
                LABEL(text);
                HR();
            } else if (type == Block::TAB) {
                *tmpPageBuf += F("<div class='blockHeader'");
                if (st != DEFAULT) {
                    *tmpPageBuf += F(" style='background:");
                    *tmpPageBuf += FPSTR(st);
                    *tmpPageBuf += "'";
                }
                *tmpPageBuf += ">";
                *tmpPageBuf += text;
                *tmpPageBuf += F("</div>\n");
            } else if (type == Block::THIN) {
                *tmpPageBuf += F("<div class='blockHeader thinTab'>");
                *tmpPageBuf += F("<span class='thinText'");
                if (st != DEFAULT) {
                    *tmpPageBuf += F(" style='color:");
                    *tmpPageBuf += FPSTR(st);
                    *tmpPageBuf += "'";
                }
                *tmpPageBuf += ">";
                *tmpPageBuf += text;
                *tmpPageBuf += F("</span></div>\n");
            }
        }
        send();
    }

    void BLOCK_BEGIN(const String& width = "") {
        BLOCK_BEGIN(Block::TAB, width);
    }

    void BLOCK_TAB_BEGIN(const String& label, const String& width = "", PGM_P st = DEFAULT) {
        BLOCK_BEGIN(Block::TAB, width, label, st);
    }
    void BLOCK_THIN_BEGIN(const String& width = "") {
        BLOCK_BEGIN(Block::THIN, width);
    }
    void BLOCK_THIN_TAB_BEGIN(const String& label, const String& width = "") {
        BLOCK_BEGIN(Block::THIN, width, label);
    }
    void BLOCK_END() { SEND(F("</div>\n")); }

    void BOX_BEGIN(Align al = Align::JUSTIFY, const String& w = "100%", bool top = 0) {
        *tmpPageBuf += F("<div style='width:");
        *tmpPageBuf += w;
        *tmpPageBuf += F(";justify-content:");
        *tmpPageBuf += FPSTR(getAlignFlex(al));
        if (top) *tmpPageBuf += F(";align-items: flex-start");
        *tmpPageBuf += F("' class='inliner'>\n");
        send();
    }
    void BOX_END() { SEND(F("</div>\n")); }
    void BREAK() { SEND(F("<br>\n")); }
    void HR() { SEND(F("<hr>\n")); }

    void HR(PGM_P st) {
        *tmpPageBuf += F("<hr style='border-color:");
        *tmpPageBuf += FPSTR(st);
        *tmpPageBuf += F("'>\n");
    }

    // ======================= ТЕКСТ =======================
    void TAG_RAW(const String& tag, const String& val, const String& name = "", PGM_P st = DEFAULT, int size = 0, bool bold = 0, bool wrap = 0, PGM_P back = DEFAULT) {
        *tmpPageBuf += F("<");
        *tmpPageBuf += tag;
        if (name.length()) {
            *tmpPageBuf += F(" id='");
            *tmpPageBuf += name;
            *tmpPageBuf += "'";
        }
        *tmpPageBuf += F(" style='");
        if (st != DEFAULT) {
            *tmpPageBuf += F("color:");
            *tmpPageBuf += FPSTR(st);
            *tmpPageBuf += ';';
        }
        if (back != DEFAULT) {
            *tmpPageBuf += F("background-color:");
            *tmpPageBuf += FPSTR(back);
            *tmpPageBuf += ';';
        }
        if (size) {
            *tmpPageBuf += F("font-size:");
            *tmpPageBuf += size;
            *tmpPageBuf += "px;";
        }
        if (bold) *tmpPageBuf += F("font-weight:bold;");
        if (wrap) *tmpPageBuf += F("white-space:normal;");
        *tmpPageBuf += F("'>");
        *tmpPageBuf += val;
        *tmpPageBuf += F("</");
        *tmpPageBuf += tag;
        *tmpPageBuf += ">\n";
        send();
    }

    void TITLE(const String& text, const String& name = "", PGM_P style = DEFAULT, int size = 0, bool bold = 0) {
        TAG_RAW(F("h2"), text, name, style, size, bold);
    }
    void LABEL(const String& text, const String& name = "", PGM_P style = DEFAULT, int size = 0, bool bold = 0, bool wrap = 0) {
        TAG_RAW(F("label"), text, name, style, size, bold, wrap);
    }
    void LABEL_BLOCK(const String& text, const String& name = "", PGM_P style = GREEN, int size = 0, bool bold = 0) {
        TAG_RAW(F("label class='display'"), text, name, DEFAULT, size, bold, 0, style);
    }

    // устарело
    void SPAN(const String& text, Align al = Align::CENTER, const String& name = "", PGM_P st = DEFAULT, int size = 0, bool bold = 0) {
        if (al != Align::CENTER) {
            *tmpPageBuf += F("<div style='text-align:");
            *tmpPageBuf += FPSTR(getAlign(al));
            *tmpPageBuf += F("'>");
        } else *tmpPageBuf += F("<div>");
        TAG_RAW(F("span"), text, name, st, size, bold);
        *tmpPageBuf += F("</div>\n");
        send();
    }
    void PLAIN(const String& text, const String& name = "", PGM_P st = DEFAULT) {
        TAG_RAW(F("p"), text, name, st);
        send();
    }
    void BOLD(const String& text, const String& name = "", PGM_P st = DEFAULT) {
        TAG_RAW(F("strong"), text, name, st);
        send();
    }
    // устарело

    // ======================= ЛЕДЫ =======================
    void LED(const String& name, bool state = 0) {
        *tmpPageBuf += F("<input class='ledn' type='radio' disabled ");
        if (state) *tmpPageBuf += F("checked ");
        *tmpPageBuf += F("name='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' id='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("'>\n");
        send();
    }
    void LED(const String& name, bool state, PGM_P st) {
        *tmpPageBuf += F("<style>.led_");
        *tmpPageBuf += name;
        *tmpPageBuf += F(":checked:after{background-color:");
        *tmpPageBuf += FPSTR(st);
        *tmpPageBuf += F(";box-shadow:inset 0px 3px #fff7,0px 0px 10px 1px ");
        *tmpPageBuf += FPSTR(st);
        *tmpPageBuf += F(";}</style>\n");

        *tmpPageBuf += F("<input class='led led_");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' type='radio' disabled ");
        if (state) *tmpPageBuf += F("checked ");
        *tmpPageBuf += F("name='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' id='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("'>\n");
        send();
    }

    // устарело
    [[deprecated("устарело")]] void LED_RED(const String& name, bool state = 0) {
        *tmpPageBuf += F("<input class='led red' type='radio' disabled ");
        if (state) *tmpPageBuf += F("checked ");
        *tmpPageBuf += F("name='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' id='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("'>\n");
        send();
    }
    [[deprecated("устарело")]] void LED_GREEN(const String& name, bool state = 0) {
        *tmpPageBuf += F("<input class='led green' type='radio' disabled ");
        if (state) *tmpPageBuf += F("checked ");
        *tmpPageBuf += F("name='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' id='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("'>\n");
        send();
    }

    // ======================= ИКОНКИ =======================
    void ICON_SUPPORT() {
        SEND(F("<link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css'>\n"));
    }

    String ICON(const String& faname, int size = 20, PGM_P st = DEFAULT) {
        String s(F("<i class='fa fa-"));
        s += faname;
        s += F("' style='");
        if (size) {
            s += F("font-size:");
            s += size;
            s += F("px;");
        }
        if (st != DEFAULT) {
            s += F("color:");
            s += FPSTR(st);
            s += ";";
        }
        s += F("'></i>");
        return s;
    }
    String ICON_FILE(const String& uri, int size = 20, PGM_P st = DEFAULT) {
        String s(F("<i class='i_mask' style='-webkit-mask:center/contain no-repeat url("));
        s += uri;
        s += F(");");
        if (st != DEFAULT) {
            s += F("background-color:");
            s += FPSTR(st);
            s += ";";
        }
        if (size) {
            s += F("width:");
            s += size;
            s += F("px;height:");
            s += size;
            s += F("px;");
        }
        s += F("'></i>");
        return s;
    }

    void ICON_BUTTON_RAW(const String& name, const String& icon) {
        *tmpPageBuf += F("<div class='i_btn' id='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' name='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' onmousedown='if(!_touch)GP_press(this,1)' onmouseup='if(!_touch&&_pressId)GP_press(this,2)' onmouseleave='if(_pressId&&!_touch)GP_press(this,2);' "
                         "ontouchstart='_touch=1;GP_press(this,1)' ontouchend='GP_press(this,2)' onclick='GP_click(this)'>");
        *tmpPageBuf += icon;
        *tmpPageBuf += F("</div>");
    }

    void ICON_BUTTON(const String& name, const String& faname, int size = 0, PGM_P st = DEFAULT) {
        ICON_BUTTON_RAW(name, ICON(faname, size, st));
    }
    void ICON_FILE_BUTTON(const String& name, const String& uri, int size = 0, PGM_P st = DEFAULT) {
        ICON_BUTTON_RAW(name, ICON_FILE(uri, size, st));
    }

    // ======================= НАВИГАЦИЯ =======================
    void NAV_TABS_LINKS(const String& urls, const String& names, PGM_P st = GREEN) {
        *tmpPageBuf += F("<div class='navtab'><ul ");
        if (st != GREEN) {
            *tmpPageBuf += F("style='background:");
            *tmpPageBuf += FPSTR(st);
            *tmpPageBuf += "'";
        }
        *tmpPageBuf += ">\n";
        Parser n(names);
        Parser u(urls);
        while (n.parse()) {
            u.parse();
            *tmpPageBuf += F("<li ");
            if (_gp_uri->equals(u.str)) *tmpPageBuf += F("style='background:#2a2d35' ");
            *tmpPageBuf += F("onclick='location.href=\"");
            *tmpPageBuf += u.str;
            *tmpPageBuf += F("\";'>");
            *tmpPageBuf += n.str;
            *tmpPageBuf += F("</li>\n");
        }
        *tmpPageBuf += F("</ul></div>\n");
        send();
    }

    void NAV_TABS_M(const String& name, const String& list, PGM_P st = GREEN) {
        _gp_nav_pos = 0;
        *tmpPageBuf += F("<div class='navtab'><ul ");
        if (st != GREEN) {
            *tmpPageBuf += F("style='background:");
            *tmpPageBuf += FPSTR(st);
            *tmpPageBuf += "' ";
        }
        *tmpPageBuf += ">\n";
        Parser p(list);
        while (p.parse()) {
            *tmpPageBuf += F("<li ");
            if (!p.count) *tmpPageBuf += F("style='background:#2a2d35' ");
            *tmpPageBuf += F("' class='");
            *tmpPageBuf += name;
            *tmpPageBuf += F("' onclick='openTab(\"");
            *tmpPageBuf += name;
            *tmpPageBuf += '/';
            *tmpPageBuf += p.count;
            *tmpPageBuf += F("\",this,\"block_");
            *tmpPageBuf += name;
            *tmpPageBuf += F("\");GP_send(\"/GP_click?");
            *tmpPageBuf += name;
            *tmpPageBuf += '/';
            *tmpPageBuf += p.count;
            *tmpPageBuf += F("=\");'>");
            *tmpPageBuf += p.str;
            *tmpPageBuf += F("</li>\n");
        }
        *tmpPageBuf += F("</ul></div>\n");
        send();
    }

    void NAV_BLOCK_BEGIN(const String& name, int pos) {
        *tmpPageBuf += F("<div class='navblock block_");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' id='");
        *tmpPageBuf += name;
        *tmpPageBuf += '/';
        *tmpPageBuf += pos;
        *tmpPageBuf += "' ";
        if (!pos) *tmpPageBuf += F("style='display:block'");
        *tmpPageBuf += ">\n";
        send();
    }

    void NAV_TABS(const String& list, PGM_P st = GREEN) {
        _gp_nav_id++;
        _gp_nav_pos = 0;
        *tmpPageBuf += F("<div class='navtab'><ul ");
        if (st != GREEN) {
            *tmpPageBuf += F("style='background:");
            *tmpPageBuf += FPSTR(st);
            *tmpPageBuf += "' ";
        }
        *tmpPageBuf += ">\n";
        Parser p(list);
        while (p.parse()) {
            *tmpPageBuf += F("<li ");
            if (!p.count) *tmpPageBuf += F("style='background:#2a2d35' ");
            *tmpPageBuf += F("class='nt-");
            *tmpPageBuf += _gp_nav_id;
            *tmpPageBuf += F("' onclick='openTab(\"ntab-");
            *tmpPageBuf += _gp_nav_id;
            *tmpPageBuf += '/';
            *tmpPageBuf += p.count;
            *tmpPageBuf += F("\",this,\"nb-");
            *tmpPageBuf += _gp_nav_id;
            *tmpPageBuf += F("\")'>");
            *tmpPageBuf += p.str;
            *tmpPageBuf += F("</li>\n");
        }
        *tmpPageBuf += F("</ul></div>\n");
        send();
    }

    void NAV_BLOCK_BEGIN() {
        *tmpPageBuf += F("<div class='navblock nb-");
        *tmpPageBuf += _gp_nav_id;
        *tmpPageBuf += F("' id='ntab-");
        *tmpPageBuf += _gp_nav_id;
        *tmpPageBuf += '/';
        *tmpPageBuf += _gp_nav_pos;
        *tmpPageBuf += "' ";
        if (!_gp_nav_pos) *tmpPageBuf += F("style='display:block'");
        *tmpPageBuf += ">\n";
        send();
        _gp_nav_pos++;
    }

    void NAV_BLOCK_END() {
        SEND(F("</div>\n"));
    }

    // ======================= ФАЙЛЫ =======================
    void FILE_UPLOAD_RAW(const String& name, const String& text = "", PGM_P st = GREEN, const String& accept = "", const String& options = "", const String& action = "/GP_upload") {
        *tmpPageBuf += F("<div id='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("'><form action='");
        *tmpPageBuf += action;
        *tmpPageBuf += F("' method='POST' enctype='multipart/form-data' id='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("_form' style='display:flex;justify-content:center;'>\n"
                         "<div id='ubtn' onclick='saveFile(\"");
        *tmpPageBuf += name;
        *tmpPageBuf += F("_inp\")'");
        if (st != GREEN) {
            *tmpPageBuf += F(" style='background:");
            *tmpPageBuf += FPSTR(st);
            *tmpPageBuf += "'";
        }
        *tmpPageBuf += ">";
        *tmpPageBuf += text;
        *tmpPageBuf += F("</div>\n"
                         "<div id='ubtnclr'><input ");
        *tmpPageBuf += options;
        *tmpPageBuf += F("name='");
        *tmpPageBuf += name;
        if (accept.length()) {
            *tmpPageBuf += F("' accept='");
            *tmpPageBuf += accept;
        }
        *tmpPageBuf += F("' id='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("_inp' type='file' onchange='GP_submId(\"");
        *tmpPageBuf += name;
        *tmpPageBuf += F("_form\")'/></div>\n"
                         "</form></div>\n");
        send();
    }

    void FILE_UPLOAD(const String& name, const String& text = "", const String& accept = "", PGM_P st = GREEN) {
        FILE_UPLOAD_RAW(name, "📄 " + text, st, accept, F("multiple "));
    }

    void FOLDER_UPLOAD(const String& name, const String& text = "", PGM_P st = GREEN) {
        FILE_UPLOAD_RAW(name, "📁 " + text, st, "", F("multiple webkitdirectory allowdirs "));
    }

    void OTA_FIRMWARE(const String& text = "OTA firmware", PGM_P st = GREEN, bool page = 0) {
        FILE_UPLOAD_RAW(F("firmware"), "🧱 " + text, st, F(".bin,.bin.gz"), "", page ? F("/ota_update") : F("/GP_OTAupload"));
    }

    void OTA_FILESYSTEM(const String& text = "OTA filesystem", PGM_P st = GREEN, bool page = 0) {
        FILE_UPLOAD_RAW(F("filesystem"), "💽 " + text, st, F(".bin,.bin.gz"), "", page ? F("/ota_update") : F("/GP_OTAupload"));
    }

    void IMAGE(const String& uri, const String& w = "") {
        *tmpPageBuf += F("<img src='");
        *tmpPageBuf += uri;
        *tmpPageBuf += F("' style='width:");
        *tmpPageBuf += w;
        *tmpPageBuf += F("'>\n");
        send();
    }
    void VIDEO(const String& uri, const String& w = "") {
        *tmpPageBuf += F("<video src='");
        *tmpPageBuf += uri;
        *tmpPageBuf += F("' style='width:");
        *tmpPageBuf += w;
        *tmpPageBuf += F("' controls>Browser doesn't support video.</video>\n");
        send();
    }
    void EMBED(const String& uri, const String& w = "100%", const String& h = "") {
        *tmpPageBuf += F("<embed src='");
        *tmpPageBuf += uri;
        *tmpPageBuf += F("' style='width:90%;border-radius:5px;background:white;width:");
        *tmpPageBuf += w;
        if (h.length()) {
            *tmpPageBuf += F(";height:");
            *tmpPageBuf += h;
        }
        *tmpPageBuf += F(";'>\n");
        send();
    }

    void CAM_STREAM(int width = 500, int port = 90) {
        *tmpPageBuf += F("<img id='_stream' style='max-height:100%;width:");
        *tmpPageBuf += width;
        *tmpPageBuf += F("px'>\n<script>window.onload=document.getElementById('_stream').src=window.location.href.slice(0,-1)+':");
        *tmpPageBuf += port;
        *tmpPageBuf += F("/';</script>\n");
        send();
    }
    void CAM_STREAM(const String& width, int port = 90) {
        *tmpPageBuf += F("<img id='_stream' style='max-height:100%;width:");
        *tmpPageBuf += width;
        *tmpPageBuf += F("'>\n<script>window.onload=document.getElementById('_stream').src=window.location.href.slice(0,-1)+':");
        *tmpPageBuf += port;
        *tmpPageBuf += F("/';</script>\n");
        send();
    }

    // ======================= ФАЙЛОВЫЙ МЕНЕДЖЕР =======================
    void _fileRow(const String& fpath, int size) {
        *tmpPageBuf += "<tr>";
        *tmpPageBuf += F("<td align='left' style='padding-right:5px'>\n"
                         "<a style='text-decoration:none' href='");
        *tmpPageBuf += fpath;
        *tmpPageBuf += F("'>");
        *tmpPageBuf += fpath;
        *tmpPageBuf += F("</a>\n<td align='right'>[");
        *tmpPageBuf += String(size / 1000.0, 1);
        *tmpPageBuf += F(" kB]\n"
                         "<td align='center'>\n"
                         "<span title='Rename' style='cursor:pointer' onclick='GP_rename(\"");
        *tmpPageBuf += fpath;
        *tmpPageBuf += F("\")'>📝</span>\n"
                         "<a style='text-decoration:none' href='");
        *tmpPageBuf += fpath;
        *tmpPageBuf += F("' download><span title='Download'>📥</span></a>\n"
                         "<span title='Delete' style='cursor:pointer' onclick='GP_delete(\"");
        *tmpPageBuf += fpath;
        *tmpPageBuf += F("\")'>❌</span>\n");
    }

    void _showFiles(fs::FS* fs, const String& path, const String& odir, [[maybe_unused]] uint8_t levels = 0) {
#ifdef ESP8266
        yield();
        Dir dir = fs->openDir(path);
        while (dir.next()) {
            if (dir.isFile() && dir.fileName().length()) {
                String fpath = '/' + path + dir.fileName();
                if (odir.length() && !fpath.startsWith(odir)) continue;
                _fileRow(fpath, dir.fileSize());
            }
            if (dir.isDirectory()) {
                String p = path;
                p += dir.fileName();
                p += '/';
                Dir sdir = fs->openDir(p);
                _showFiles(fs, p, odir);
            }
        }

#else // ESP32
        File root = fs->open(path.length() ? path.c_str() : ("/"));
        if (!root || !root.isDirectory()) return;
        File file;
        while (file = root.openNextFile()) {
            if (file.isDirectory()) {
                if (levels) _showFiles(fs, file.path(), odir, levels - 1);
            } else {
                String fpath = path + '/' + file.name();
                if (odir.length() && !fpath.startsWith(odir)) continue;
                _fileRow(fpath, file.size());
            }
        }
#endif
    }

    void FILE_MANAGER(fs::FS* fs, const String& odir = "") {
        *tmpPageBuf += F("<table>");
        _showFiles(fs, "", odir, 5);

#ifdef ESP8266
        FSInfo fsi;
        fs->info(fsi);
        *tmpPageBuf += F("<tr><td colspan=3 align=center><hr><strong>Used ");
        *tmpPageBuf += String(fsi.usedBytes / 1000.0, 2);
        *tmpPageBuf += F(" kB from ");
        *tmpPageBuf += String(fsi.totalBytes / 1000.0, 2);
        *tmpPageBuf += F(" (");
        *tmpPageBuf += (fsi.usedBytes * 100 / fsi.totalBytes);
        *tmpPageBuf += F("%)</strong>");
#endif
        *tmpPageBuf += F("</table>\n");
        send();
    }

    // ================ СИСТЕМНАЯ ИНФОРМАЦИЯ ================
    void SYSTEM_INFO(const String& fwv = "", const String& w = "") {
        TABLE_BEGIN(w);
        // ===========
        TR();
        TD(Align::CENTER, 3);
        LABEL(F("Network"));
        HR();

        TR();
        TD(Align::LEFT);
        BOLD(F("WiFi Mode"));
        TD(Align::RIGHT);
        SEND(WiFi.getMode() == WIFI_AP ? F("AP") : (WiFi.getMode() == WIFI_STA ? F("STA") : F("AP_STA")));

        if (WiFi.getMode() != WIFI_AP) {
            TR();
            TD(Align::LEFT);
            BOLD(F("SSID"));
            TD(Align::RIGHT);
            SEND(WiFi.SSID());

            TR();
            TD(Align::LEFT);
            BOLD(F("Local IP"));
            TD(Align::RIGHT);
            SEND(WiFi.localIP().toString());
        }
        if (WiFi.getMode() != WIFI_STA) {
            TR();
            TD(Align::LEFT);
            BOLD(F("AP IP"));
            TD(Align::RIGHT);
            SEND(WiFi.softAPIP().toString());
        }

        if (_gp_mdns && strlen(_gp_mdns)) {
            TR();
            TD(Align::LEFT);
            BOLD(F("mDNS"));
            TD(Align::RIGHT);
            SEND(String(_gp_mdns) + ".local");
        }

        TR();
        TD(Align::LEFT);
        BOLD(F("Subnet"));
        TD(Align::RIGHT);
        SEND(WiFi.subnetMask().toString());

        TR();
        TD(Align::LEFT);
        BOLD(F("Gateway"));
        TD(Align::RIGHT);
        SEND(WiFi.gatewayIP().toString());

        TR();
        TD(Align::LEFT);
        BOLD(F("MAC Address"));
        TD(Align::RIGHT);
        SEND(WiFi.macAddress());

        TR();
        TD(Align::LEFT);
        BOLD(F("RSSI"));
        TD(Align::RIGHT);
        SEND("📶 " + String(constrain(2 * (WiFi.RSSI() + 100), 0, 100)) + '%');

        // ===========
        TR();
        TD(Align::CENTER, 3);
        LABEL(F("Memory"));
        HR();

        TR();
        TD(Align::LEFT);
        BOLD(F("Free Heap"));
        TD(Align::RIGHT);
        SEND(String(ESP.getFreeHeap() / 1000.0, 3) + " kB");

#ifdef ESP8266
        TR();
        TD(Align::LEFT);
        BOLD(F("Heap Fragm."));
        TD(Align::RIGHT);
        SEND(String(ESP.getHeapFragmentation()) + '%');
#endif

        TR();
        TD(Align::LEFT);
        BOLD(F("Sketch Size (Free)"));
        TD(Align::RIGHT);
        SEND(String(ESP.getSketchSize() / 1000.0, 1) + " kB (" + String(ESP.getFreeSketchSpace() / 1000.0, 1) + ")");

        TR();
        TD(Align::LEFT);
        BOLD(F("Flash Size"));
        TD(Align::RIGHT);
        SEND(String(ESP.getFlashChipSize() / 1000.0, 1) + " kB");

        // ===========
        TR();
        TD(Align::CENTER, 3);
        LABEL(F("System"));
        HR();

#ifdef ESP8266
        TR();
        TD(Align::LEFT);
        BOLD(F("Chip ID"));
        TD(Align::RIGHT);
        SEND("0x" + String(ESP.getChipId(), HEX));
#endif

        TR();
        TD(Align::LEFT);
        BOLD(F("Cycle Count"));
        TD(Align::RIGHT);
        SEND(String(ESP.getCycleCount()));

        TR();
        TD(Align::LEFT);
        BOLD(F("Cpu Freq."));
        TD(Align::RIGHT);
        SEND(String(ESP.getCpuFreqMHz()) + F(" MHz"));

        TR();
        TD(Align::LEFT);
        BOLD(F("Date"));
        GP::Date date(_gp_local_unix);
        TD(Align::RIGHT);
        SEND(_gp_local_unix ? date.encodeDMY() : String("unset"));

        TR();
        TD(Align::LEFT);
        BOLD(F("Time"));
        GP::Time time(_gp_local_unix);
        TD(Align::RIGHT);
        SEND(_gp_local_unix ? time.encode() : String("unset"));

        TR();
        TD(Align::LEFT);
        BOLD(F("Uptime"));
        uint32_t sec = millis() / 1000ul;
        uint8_t second = sec % 60ul;
        sec /= 60ul;
        uint8_t minute = sec % 60ul;
        sec /= 60ul;
        uint16_t hour = sec % 24ul;
        sec /= 24ul;
        String s;
        s.reserve(10);
        s += sec; // day
        s += ':';
        s += hour;
        s += ':';
        s += minute / 10;
        s += minute % 10;
        s += ':';
        s += second / 10;
        s += second % 10;
        TD(Align::RIGHT);
        SEND(s);

        // ===========
        TR();
        TD(Align::CENTER, 3);
        LABEL(F("Version"));
        HR();

        TR();
        TD(Align::LEFT);
        BOLD(F("SDK"));
        TD(Align::RIGHT);
        SEND(ESP.getSdkVersion());

#ifdef ESP8266
        TR();
        TD(Align::LEFT);
        BOLD(F("Core"));
        TD(Align::RIGHT);
        SEND(ESP.getCoreVersion());
#endif

        TR();
        TD(Align::LEFT);
        BOLD(F("GyverPortal"));
        TD(Align::RIGHT);
        SEND(GP_VERSION);

        if (fwv.length()) {
            TR();
            TD(Align::LEFT);
            BOLD(F("Firmware"));
            TD(Align::RIGHT);
            SEND(fwv);
        }

        TABLE_END();
    }

    // ======================= КНОПКА =======================
    void BUTTON_RAW(const String& name, const String& value, const String& tar, PGM_P st, const String& width = "", const String& cls = "", bool dis = 0, bool rel = 0) {
        *tmpPageBuf += F("<button type='button' ");
        if (cls.length()) {
            *tmpPageBuf += F("class='");
            *tmpPageBuf += cls;
            *tmpPageBuf += "' ";
        }
        *tmpPageBuf += F("style='");
        if (st != GREEN) {
            *tmpPageBuf += F("background:");
            *tmpPageBuf += FPSTR(st);
            *tmpPageBuf += ';';
        }
        if (width.length()) {
            *tmpPageBuf += F("width:");
            *tmpPageBuf += width;
            *tmpPageBuf += ';';
        }
        *tmpPageBuf += F("' name='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' id='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' onmousedown='if(!_touch)GP_press(this,1)' onmouseup='if(!_touch&&_pressId)GP_press(this,2)' onmouseleave='if(_pressId&&!_touch)GP_press(this,2);' ");
        if (!dis) *tmpPageBuf += F("ontouchstart='_touch=1;GP_press(this,1)' ontouchend='GP_press(this,2)' ");
        if (tar.length()) {
            *tmpPageBuf += F("onclick=\"GP_clickid('");
            *tmpPageBuf += name;
            *tmpPageBuf += F("','");
            *tmpPageBuf += tar;
            *tmpPageBuf += F("')\"");
        } else {
            *tmpPageBuf += F("onclick='GP_click(this,");
            *tmpPageBuf += rel;
            *tmpPageBuf += F(")'");
        }
        if (dis) *tmpPageBuf += F(" disabled");
        *tmpPageBuf += ">";
        *tmpPageBuf += value;
        *tmpPageBuf += F("</button>\n");
        send();
    }

    void BUTTON(const String& name, const String& value, const String& tar = "", PGM_P st = GREEN, const String& width = "", bool dis = 0, bool rel = 0) {
        BUTTON_RAW(name, value, tar, st, width, "", dis, rel);
    }
    void BUTTON_MINI(const String& name, const String& value, const String& tar = "", PGM_P st = GREEN, const String& width = "", bool dis = 0, bool rel = 0) {
        BUTTON_RAW(name, value, tar, st, width, F("miniButton"), dis, rel);
    }

    // ======================= КНОПКА-ССЫЛКА =======================
    void BUTTON_LINK_RAW(const String& url, const String& value, PGM_P st = GREEN, const String& width = "", const String& cls = "", const String& name = "") {
        *tmpPageBuf += F("<input type='button' ");
        if (name.length()) {
            *tmpPageBuf += F("name='");
            *tmpPageBuf += name;
            *tmpPageBuf += F("' id='");
            *tmpPageBuf += name;
            *tmpPageBuf += "' ";
        }
        if (cls.length()) {
            *tmpPageBuf += F("class='");
            *tmpPageBuf += cls;
            *tmpPageBuf += "' ";
        }
        *tmpPageBuf += F("style='");
        if (st != GREEN) {
            *tmpPageBuf += F("background:");
            *tmpPageBuf += FPSTR(st);
            *tmpPageBuf += ';';
        }
        if (width.length()) {
            *tmpPageBuf += F("width:");
            *tmpPageBuf += width;
            *tmpPageBuf += ';';
        }
        *tmpPageBuf += F("' value='");
        *tmpPageBuf += value;

        if (name.length()) {
            *tmpPageBuf += F("' onclick='GP_click(this,\"");
            *tmpPageBuf += url;
            *tmpPageBuf += F("\");'>\n");
        } else {
            *tmpPageBuf += F("' onclick='location.href=\"");
            *tmpPageBuf += url;
            *tmpPageBuf += F("\";'>\n");
        }
        send();
    }
    void BUTTON_LINK(const String& url, const String& value, PGM_P st = GREEN, const String& width = "", const String& name = "") {
        BUTTON_LINK_RAW(url, value, st, width, "", name);
    }
    void BUTTON_MINI_LINK(const String& url, const String& value, PGM_P st = GREEN, const String& width = "", const String& name = "") {
        BUTTON_LINK_RAW(url, value, st, width, F("miniButton"), name);
    }

    // ==================== КНОПКА-СКАЧКА ====================
    void BUTTON_DOWNLOAD_RAW(const String& url, const String& value, PGM_P st = GREEN, const String& width = "", const String& cls = "") {
        *tmpPageBuf += F("<a style='text-decoration:none;' href='");
        *tmpPageBuf += url;
        *tmpPageBuf += F("' download><input type='button' value='");
        *tmpPageBuf += value;
        *tmpPageBuf += "' ";
        if (cls.length()) {
            *tmpPageBuf += F("class='");
            *tmpPageBuf += cls;
            *tmpPageBuf += "' ";
        }
        *tmpPageBuf += F("style='");
        if (st != GREEN) {
            *tmpPageBuf += F("background:");
            *tmpPageBuf += FPSTR(st);
            *tmpPageBuf += ';';
        }
        if (width.length()) {
            *tmpPageBuf += F("width:");
            *tmpPageBuf += width;
            *tmpPageBuf += ';';
        }
        *tmpPageBuf += F("'>");
        *tmpPageBuf += F("</a>\n");
        send();
    }
    void BUTTON_DOWNLOAD(const String& url, const String& value, PGM_P st = GREEN, const String& width = "") {
        BUTTON_DOWNLOAD_RAW(url, value, st, width);
    }
    void BUTTON_MINI_DOWNLOAD(const String& url, const String& value, PGM_P st = GREEN, const String& width = "") {
        BUTTON_DOWNLOAD_RAW(url, value, st, width, F("miniButton"));
    }

    // ========================= ВВОД ========================
    void NUMBER_RAW(const String& name, const String& place = "", const String& value = "", const String& minv = "", const String& maxv = "", const String& width = "", const String& pattern = "", bool dis = 0) {
        *tmpPageBuf += F("<input type='number' step='any' name='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' id='");
        *tmpPageBuf += name;
        if (value.length()) {
            *tmpPageBuf += F("' value='");
            *tmpPageBuf += value;
        }
        if (width.length()) {
            *tmpPageBuf += F("' style='width:");
            *tmpPageBuf += width;
        }
        if (minv.length()) {
            *tmpPageBuf += F("' min='");
            *tmpPageBuf += minv;
        }
        if (maxv.length()) {
            *tmpPageBuf += F("' max='");
            *tmpPageBuf += maxv;
        }
        if (pattern.length()) {
            *tmpPageBuf += F("' pattern=");
            *tmpPageBuf += pattern;
        }
        *tmpPageBuf += F("' placeholder='");
        *tmpPageBuf += place;
        *tmpPageBuf += F("' onchange='GP_click(this)'");
        if (dis) *tmpPageBuf += F(" disabled");
        *tmpPageBuf += ">\n";
        send();
    }
    void NUMBER(const String& name, const String& place = "", int value = INT32_MAX, const String& width = "", bool dis = false) {
        NUMBER_RAW(name, place, (value == INT32_MAX ? String("") : String(value)), "", "", width, "", dis);
    }
    void NUMBER_F(const String& name, const String& place = "", float value = NAN, uint8_t dec = 2, const String& width = "", bool dis = false) {
        NUMBER_RAW(name, place, (isnan(value) ? String("") : String(value, (uint16_t)dec)), "", "", width, "", dis);
    }

    void TEXT(const String& name, const String& place = "", const String& value = "", const String& width = "", int maxlength = 0, const String& pattern = "", bool dis = false) {
        *tmpPageBuf += F("<input type='text' name='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' id='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' value='");
        *tmpPageBuf += value;
        if (width.length()) {
            *tmpPageBuf += F("' style='width:");
            *tmpPageBuf += width;
        }
        *tmpPageBuf += F("' placeholder='");
        *tmpPageBuf += place;
        *tmpPageBuf += F("' onchange='GP_click(this)'");
        if (dis) *tmpPageBuf += F(" disabled");
        if (maxlength) {
            *tmpPageBuf += F(" maxlength=");
            *tmpPageBuf += maxlength;
        }
        if (pattern.length()) {
            *tmpPageBuf += F(" pattern=");
            *tmpPageBuf += pattern;
        }
        *tmpPageBuf += ">\n";
        send();
    }
    void PASS(const String& name, const String& place = "", const String& value = "", const String& width = "", int maxlength = 0, const String& pattern = "", bool dis = false, bool eye = 0) {
        *tmpPageBuf += F("<div class='inlBlock'><input type='password' name='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' id='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' value='");
        *tmpPageBuf += value;
        if (width.length()) {
            *tmpPageBuf += F("' style='width:");
            *tmpPageBuf += width;
        }
        *tmpPageBuf += F("' placeholder='");
        *tmpPageBuf += place;
        *tmpPageBuf += F("' onchange='GP_click(this)'");
        if (dis) *tmpPageBuf += F(" disabled");
        if (maxlength) {
            *tmpPageBuf += F(" maxlength=");
            *tmpPageBuf += maxlength;
        }
        if (pattern.length()) {
            *tmpPageBuf += F(" pattern=");
            *tmpPageBuf += pattern;
        }
        *tmpPageBuf += ">\n";
        if (eye) *tmpPageBuf += F("<span class='eyepass' onclick='GP_eye(this)'>&#x1f441;&#xFE0E;</span>");
        *tmpPageBuf += F("</div>\n");
        send();
    }
    void PASS_EYE(const String& name, const String& place = "", const String& value = "", const String& width = "", int maxlength = 0, const String& pattern = "", bool dis = false) {
        PASS(name, place, value, width, maxlength, pattern, dis, true);
    }

    void AREA(const String& name, int rows = 1, const String& value = "", const String& width = "", bool dis = false) {
        *tmpPageBuf += F("<textarea onchange='GP_click(this)' style='height:auto' name='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' id='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' rows='");
        *tmpPageBuf += rows;
        if (width.length()) {
            *tmpPageBuf += F("' style='width:");
            *tmpPageBuf += width;
        }
        *tmpPageBuf += "'";
        if (dis) *tmpPageBuf += F(" disabled");
        *tmpPageBuf += ">";
        if (value.length()) *tmpPageBuf += value;
        *tmpPageBuf += F("</textarea>\n");
        send();
    }

    void AREA_LOG_RAW(const String& name, int rows = 5, int prd = 1000, const String& w = "") {
        *tmpPageBuf += F("<div class='inlBlock'><textarea name='_gplog' style='height:auto;");
        if (w.length()) {
            *tmpPageBuf += F("width:");
            *tmpPageBuf += w;
        }
        *tmpPageBuf += F("' id='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' rows='");
        *tmpPageBuf += rows;
        *tmpPageBuf += F("' disabled></textarea>\n");
        *tmpPageBuf += F("<div class='ctrlBlock'><span class='areaCtrl' style='color:red' onclick='logClear(\"");
        *tmpPageBuf += name;
        *tmpPageBuf += F("\")'>x</span><span class='areaCtrl' style='color:#888' onclick='logToggle(\"");
        *tmpPageBuf += name;
        *tmpPageBuf += F("\")'>s</span></div>\n");
        *tmpPageBuf += F("</div>\n");

        JS_BEGIN();
        *tmpPageBuf += F("setInterval(()=>GP_update('");
        *tmpPageBuf += name;
        *tmpPageBuf += F("'),");
        *tmpPageBuf += prd;
        *tmpPageBuf += F(");");
        JS_END();
        send();
    }
    void AREA_LOG(int rows = 5, int prd = 1000, const String& w = "") {
        AREA_LOG_RAW("GP_log", rows, prd, w);
    }
    void AREA_LOG(GP::Log& log, int rows = 5, int prd = 1000, const String& w = "") {
        // log.clear();
        AREA_LOG_RAW(log.name, rows, prd, w);
    }

    // ======================= НАСТРОЙКА =======================
    void CHECK(const String& name, bool state = 0, PGM_P st = GREEN, bool dis = false) {
        if (st != GREEN) {
            *tmpPageBuf += F("<style>#__");
            *tmpPageBuf += name;
            *tmpPageBuf += F(" input:checked+span::before{border-color:");
            *tmpPageBuf += FPSTR(st);
            *tmpPageBuf += F(";background-color:");
            *tmpPageBuf += FPSTR(st);
            *tmpPageBuf += F("}</style>\n");
        }
        *tmpPageBuf += F("<label id='__");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' class='check_c'><input type='checkbox' name='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' id='");
        *tmpPageBuf += name;
        *tmpPageBuf += "' ";
        if (state) *tmpPageBuf += F("checked ");
        if (dis) *tmpPageBuf += F("disabled ");
        *tmpPageBuf += F("onclick='GP_click(this)'><span></span></label>\n"
                         "<input type='hidden' value='0' name='");
        *tmpPageBuf += name;
        *tmpPageBuf += "'>\n";
        send();
    }
    void SWITCH(const String& name, bool state = 0, PGM_P st = GREEN, bool dis = false) {
        if (st != GREEN) {
            *tmpPageBuf += F("<style>#__");
            *tmpPageBuf += name;
            *tmpPageBuf += F(" input:checked+.slider{background-color:");
            *tmpPageBuf += FPSTR(st);
            *tmpPageBuf += F("}</style>\n");
        }
        *tmpPageBuf += F("<label id='__");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' class='switch'><input class='_sw_c' type='checkbox' name='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' id='");
        *tmpPageBuf += name;
        *tmpPageBuf += "' ";
        if (state) *tmpPageBuf += F("checked ");
        if (dis) *tmpPageBuf += F("disabled ");
        *tmpPageBuf += F("onclick='GP_click(this)'>\n"
                         "<span class='slider' id='_");
        *tmpPageBuf += name;
        *tmpPageBuf += F("'></span></label>\n"
                         "<input type='hidden' value='0' name='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("'>\n");
        send();
    }

    void DATE(const String& name, bool dis = false) {
        GP::Date d;
        DATE(name, d, dis);
    }
    void DATE(const String& name, GP::Date d, bool dis = false) {
        *tmpPageBuf += F("<input step='any' type='date' name='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' id='");
        *tmpPageBuf += name;
        if (d.year) {
            *tmpPageBuf += F("' value='");
            *tmpPageBuf += d.encode();
        }
        *tmpPageBuf += "' ";
        if (dis) *tmpPageBuf += F("disabled ");
        *tmpPageBuf += F("onchange='GP_click(this)'>\n");
        send();
    }

    void TIME(const String& name, bool dis = false) {
        *tmpPageBuf += F("<input step='1' type='time' name='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' id='");
        *tmpPageBuf += name;
        *tmpPageBuf += "' ";
        if (dis) *tmpPageBuf += F("disabled ");
        *tmpPageBuf += F("onchange='GP_click(this)'>\n");
        send();
    }
    void TIME(const String& name, GP::Time t, bool dis = false) {
        *tmpPageBuf += F("<input step='1' type='time' name='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' id='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' value='");
        *tmpPageBuf += t.encode();
        *tmpPageBuf += "' ";
        if (dis) *tmpPageBuf += F("disabled ");
        *tmpPageBuf += F("onchange='GP_click(this)'>\n");
        send();
    }

    void _FLOAT_DEC(float val, uint16_t dec) {
        if (!dec) *tmpPageBuf += (int)round(val);
        else *tmpPageBuf += String(val, (uint16_t)dec);
    }
    void SLIDER(const String& name, float value = 0, float min = 0, float max = 100, float step = 1, uint8_t dec = 0, PGM_P st = GREEN, bool dis = 0, bool oninp = 0) {
        *tmpPageBuf += F("<input type='range' name='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' id='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' value='");
        *tmpPageBuf += value;
        *tmpPageBuf += F("' min='");
        *tmpPageBuf += min;
        *tmpPageBuf += F("' max='");
        *tmpPageBuf += max;
        *tmpPageBuf += F("' step='");
        *tmpPageBuf += step;
        *tmpPageBuf += F("' style='background-image:linear-gradient(");
        *tmpPageBuf += FPSTR(st);
        *tmpPageBuf += ',';
        *tmpPageBuf += FPSTR(st);
        *tmpPageBuf += F(");background-size:0% 100%' onload='GP_change(this)' ");
        if (oninp) *tmpPageBuf += F("oninput='GP_change(this);GP_click(this)'");
        else *tmpPageBuf += F("onchange='GP_click(this)' oninput='GP_change(this)'");
        *tmpPageBuf += F(" onmousewheel='GP_wheel(this);GP_change(this);GP_click(this)' ");
        if (dis) *tmpPageBuf += F("disabled");
        *tmpPageBuf += ">\n";
        *tmpPageBuf += F("<output align='center' id='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("_val' ");
        if (st != GREEN) {
            *tmpPageBuf += F("style='background:");
            *tmpPageBuf += FPSTR(st);
            *tmpPageBuf += F(";'");
        }
        if (dis) *tmpPageBuf += F(" class='dsbl'");
        *tmpPageBuf += F(">");
        _FLOAT_DEC(value, dec);
        *tmpPageBuf += F("</output>\n");
        send();
    }

    void SLIDER_C(const String& name, float value = 0, float min = 0, float max = 100, float step = 1, uint8_t dec = 0, PGM_P st = GREEN, bool dis = 0) {
        SLIDER(name, value, min, max, step, dec, st, dis, 1);
    }

    void SPIN_BTN(const String& name, float step, PGM_P st, uint8_t dec, bool dis) {
        *tmpPageBuf += F("<input type='button' class='spinBtn ");
        *tmpPageBuf += (step > 0) ? F("spinR") : F("spinL");
        *tmpPageBuf += F("' name='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' min='"); // step
        *tmpPageBuf += step;
        *tmpPageBuf += F("' max='"); // dec
        *tmpPageBuf += dec;
        *tmpPageBuf += F("' onmouseleave='if(_pressId)clearInterval(_spinInt);_spinF=_pressId=null' onmousedown='_pressId=this.name;_spinInt=setInterval(()=>{GP_spin(this);_spinF=1},");
        *tmpPageBuf += _spinInt;
        *tmpPageBuf += F(")' onmouseup='clearInterval(_spinInt)' onclick='if(!_spinF)GP_spin(this);_spinF=0' value='");
        *tmpPageBuf += (step > 0) ? '+' : '-';
        *tmpPageBuf += "' ";
        if (st != GREEN) {
            *tmpPageBuf += F(" style='background:");
            *tmpPageBuf += FPSTR(st);
            *tmpPageBuf += F(";'");
        }
        if (dis) *tmpPageBuf += F(" disabled");
        *tmpPageBuf += ">\n";
    }

    void SPINNER(const String& name, float value = 0, float min = NAN, float max = NAN, float step = 1, uint16_t dec = 0, PGM_P st = GREEN, const String& w = "", bool dis = 0) {
        *tmpPageBuf += F("<div id='spinner' class='spinner'>\n");
        SPIN_BTN(name, -step, st, dec, dis);
        *tmpPageBuf += F("<input type='number' name='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' id='");
        *tmpPageBuf += name;
        if (w.length()) {
            *tmpPageBuf += F("' style='width:");
            *tmpPageBuf += w;
        }
        *tmpPageBuf += F("' step='");
        _FLOAT_DEC(step, dec);
        *tmpPageBuf += F("' onkeyup='GP_spinw(this)' onkeydown='GP_spinw(this)' onchange='GP_spinw(this);GP_click(this)' onmousewheel='GP_spinw(this);GP_wheel(this);GP_click(this)' value='");
        _FLOAT_DEC(value, dec);
        if (!isnan(min)) {
            *tmpPageBuf += F("' min='");
            _FLOAT_DEC(min, dec);
        }
        if (!isnan(max)) {
            *tmpPageBuf += F("' max='");
            _FLOAT_DEC(max, dec);
        }
        *tmpPageBuf += "' ";
        if (dis) *tmpPageBuf += F("disabled ");
        if (!w.length()) *tmpPageBuf += F("class='spin_inp'");
        *tmpPageBuf += ">\n";
        SPIN_BTN(name, step, st, dec, dis);
        *tmpPageBuf += F("</div>\n");
        send();
    }

    void COLOR(const String& name, uint32_t value = 0, bool dis = false) {
        *tmpPageBuf += F("<input type='color' name='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' id='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' value='");
        GPcolor col(value);
        *tmpPageBuf += col.encode();
        *tmpPageBuf += "' ";
        if (dis) *tmpPageBuf += F("disabled ");
        *tmpPageBuf += F("onchange='GP_click(this)'>\n");
        send();
    }
    void COLOR(const String& name, GPcolor col, bool dis = false) {
        *tmpPageBuf += F("<input type='color' name='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' id='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' value='");
        *tmpPageBuf += col.encode();
        *tmpPageBuf += "' ";
        if (dis) *tmpPageBuf += F("disabled ");
        *tmpPageBuf += F("onchange='GP_click(this)'>\n");
        send();
    }

    void RADIO(const String& name, int num, int val = -1, PGM_P st = DEFAULT, bool dis = 0) {
        if (st != DEFAULT) {
            *tmpPageBuf += F("<style>.rad_");
            *tmpPageBuf += name;
            *tmpPageBuf += F(":after{border-color:");
            *tmpPageBuf += FPSTR(st);
            *tmpPageBuf += F("}.rad_");
            *tmpPageBuf += name;
            *tmpPageBuf += F(":checked:after{background-color:");
            *tmpPageBuf += FPSTR(st);
            *tmpPageBuf += F("}</style>\n");
        }

        *tmpPageBuf += F("<input class='rad rad_");
        *tmpPageBuf += name;
        if (dis) *tmpPageBuf += F(" dsbl");
        *tmpPageBuf += F("' type='radio' name='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' id='");
        *tmpPageBuf += name;
        *tmpPageBuf += '_';
        *tmpPageBuf += num;
        *tmpPageBuf += F("' value='");
        *tmpPageBuf += num;
        *tmpPageBuf += F("' onchange='GP_click(this)'");
        if (val == num) *tmpPageBuf += F(" checked");
        if (dis) *tmpPageBuf += F(" disabled");
        *tmpPageBuf += ">\n";
        send();
    }

    void SELECT(const String& name, const String& list, int sel = 0, bool nums = 0, bool dis = 0, bool rel = 0) {
        if (sel < 0) sel = 0;
        *tmpPageBuf += F("<select name='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' id='");
        *tmpPageBuf += name;
        *tmpPageBuf += "' ";
        if (dis) *tmpPageBuf += F("disabled ");
        *tmpPageBuf += F("onchange='GP_click(this,");
        *tmpPageBuf += rel;
        *tmpPageBuf += F(")'>\n");

        Parser p(list);
        int idx = 0;
        while (p.parse()) {
            *tmpPageBuf += F("<option value='");
            *tmpPageBuf += idx;
            *tmpPageBuf += "'";
            if (p.count == sel) *tmpPageBuf += F(" selected");
            *tmpPageBuf += F(">");
            if (nums) {
                *tmpPageBuf += idx;
                *tmpPageBuf += ". ";
            }
            *tmpPageBuf += p.str;
            *tmpPageBuf += F("</option>\n");
            idx++;
        }
        *tmpPageBuf += F("</select>\n");
        send();
    }
    void SELECT(const String& name, String* list, int sel = 0, bool nums = 0, bool dis = false, bool rel = 0) {
        if (sel < 0) sel = 0;
        *tmpPageBuf += F("<select name='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' id='");
        *tmpPageBuf += name;
        *tmpPageBuf += "' ";
        if (dis) *tmpPageBuf += F("disabled ");
        *tmpPageBuf += F("onchange='GP_click(this,");
        *tmpPageBuf += rel;
        *tmpPageBuf += F(")'>\n");
        int idx = 0;
        while (list[idx].length()) {
            *tmpPageBuf += F("<option value='");
            *tmpPageBuf += idx;
            *tmpPageBuf += "'";
            if (idx == sel) *tmpPageBuf += F(" selected");
            *tmpPageBuf += F(">");
            if (nums) {
                *tmpPageBuf += idx;
                *tmpPageBuf += ". ";
            }
            *tmpPageBuf += list[idx];
            *tmpPageBuf += F("</option>\n");
            idx++;
        }
        *tmpPageBuf += F("</select>\n");
        send();
    }
    void SELECT(const String& name, char** list, int sel = 0, bool nums = 0, bool dis = false, bool rel = 0) {
        if (sel < 0) sel = 0;
        *tmpPageBuf += F("<select name='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' id='");
        *tmpPageBuf += name;
        *tmpPageBuf += "' ";
        if (dis) *tmpPageBuf += F("disabled ");
        *tmpPageBuf += F("onchange='GP_click(this,");
        *tmpPageBuf += rel;
        *tmpPageBuf += F(")'>\n");
        int idx = 0;
        while (list[idx] != nullptr) {
            *tmpPageBuf += F("<option value='");
            *tmpPageBuf += idx;
            *tmpPageBuf += "'";
            if (idx == sel) *tmpPageBuf += F(" selected");
            *tmpPageBuf += F(">");
            if (nums) {
                *tmpPageBuf += idx;
                *tmpPageBuf += ". ";
            }
            *tmpPageBuf += list[idx];
            *tmpPageBuf += F("</option>\n");
            idx++;
        }
        *tmpPageBuf += F("</select>\n");
        send();
    }

    // ======================= ГРАФИКИ =======================
    template <int ax, int am>
    void PLOT(const String& id, const char** labels, int16_t vals[ax][am], int dec = 0, int height = 400) {
        *tmpPageBuf += F("<script type='text/javascript' src='https://www.gstatic.com/charts/loader.js'></script>\n"
                         "<script type='text/javascript'>\n"
                         "google.charts.load('current', {'packages':['corechart']});\n"
                         "google.charts.setOnLoadCallback(drawChart);\n"
                         "function drawChart() {\n"
                         "var data = google.visualization.arrayToDataTable([\n");
        send();

        *tmpPageBuf += '[';
        for (int i = 0; i < ax + 1; i++) {
            *tmpPageBuf += '\'';
            if (i) *tmpPageBuf += labels[i - 1];
            *tmpPageBuf += "',";
        }
        *tmpPageBuf += "],\n";
        for (int j = 0; j < am; j++) {
            *tmpPageBuf += '[';
            for (int i = 0; i < ax + 1; i++) {
                if (!i) *tmpPageBuf += '\'';
                if (!i) *tmpPageBuf += j;
                else if (dec) *tmpPageBuf += (float)vals[i - 1][j] / dec;
                else *tmpPageBuf += vals[i - 1][j];
                if (!i) *tmpPageBuf += '\'';
                *tmpPageBuf += ',';
            }
            *tmpPageBuf += F("],\n");
        }
        send();

        *tmpPageBuf += F("]);var options = {pointSize:5,curveType:'function','chartArea':{'width':'90%','height':'90%'},\n"
                         "backgroundColor:'none',hAxis:{title:''},vAxis:{title:''},\n"
                         "legend: {position:'bottom'}};\n"
                         "var chart = new google.visualization.LineChart(document.getElementById('");
        *tmpPageBuf += id;
        *tmpPageBuf += F("'));\n");
        *tmpPageBuf += F("chart.draw(data, options);}\n"
                         "</script><div id='");
        *tmpPageBuf += id;
        *tmpPageBuf += F("' class='chartBlock' style='height:");
        *tmpPageBuf += height;
        *tmpPageBuf += F("px'></div>\n");
        send();
    }

    template <int ax, int am>
    void PLOT_DARK(const String& id, const char** labels, int16_t vals[ax][am], int dec = 0, int height = 400) {
        *tmpPageBuf += F("<script type='text/javascript' src='https://www.gstatic.com/charts/loader.js'></script>\n"
                         "<script type='text/javascript'>\n"
                         "google.charts.load('current', {'packages':['corechart']});\n"
                         "google.charts.setOnLoadCallback(drawChart);\n"
                         "function drawChart() {\n"
                         "var data = google.visualization.arrayToDataTable([\n");
        send();

        *tmpPageBuf += '[';
        for (int i = 0; i < ax + 1; i++) {
            *tmpPageBuf += '\'';
            if (i) *tmpPageBuf += labels[i - 1];
            *tmpPageBuf += "',";
        }
        *tmpPageBuf += "],\n";
        for (int j = 0; j < am; j++) {
            *tmpPageBuf += '[';
            for (int i = 0; i < ax + 1; i++) {
                if (!i) *tmpPageBuf += '\'';
                if (!i) *tmpPageBuf += j;
                else if (dec) *tmpPageBuf += (float)vals[i - 1][j] / dec;
                else *tmpPageBuf += vals[i - 1][j];
                if (!i) *tmpPageBuf += '\'';
                *tmpPageBuf += ',';
            }
            *tmpPageBuf += F("],\n");
        }
        send();

        *tmpPageBuf += F("]);var options = {pointSize:5,curveType:'function','chartArea':{'width':'90%','height':'90%'},\n"
                         "backgroundColor:'none',hAxis:{title:'',titleTextStyle:{color:'#ddd'},textStyle:{color:'#bbb'}},\n"
                         "vAxis:{title:'',titleTextStyle:{color:'#ddd'},gridlines:{color:'#777'},textStyle:{color:'#bbb'}},\n"
                         "legend: {position:'bottom',textStyle:{color:'#eee'}}};\n"
                         "var chart = new google.visualization.LineChart(document.getElementById('");
        *tmpPageBuf += id;
        *tmpPageBuf += F("'));\n");
        *tmpPageBuf += F("chart.draw(data, options);}\n");
        *tmpPageBuf += F("</script><div id='");
        *tmpPageBuf += id;
        *tmpPageBuf += F("' class='chartBlock' style='height:");
        *tmpPageBuf += height;
        *tmpPageBuf += F("px'></div>\n");
        send();
    }

    void AJAX_PLOT(const String& name, int axes, int xamount = 20, int prd = 1000, int height = 400, bool local = 0) {
        *tmpPageBuf += F("<script src='");

        if (local) *tmpPageBuf += F("/gp_data/AJAX_PLOT.js");
        else *tmpPageBuf += F("https://code.highcharts.com/highcharts.js");

        *tmpPageBuf += F("'></script>\n<div id='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' class='chartBlock' style='height:");
        *tmpPageBuf += height;
        *tmpPageBuf += F("px'></div><script>\n var ");
        *tmpPageBuf += name;
        *tmpPageBuf += F("=new Highcharts.Chart({\n"
                         "chart:{borderRadius:10,renderTo:'");
        *tmpPageBuf += name;
        *tmpPageBuf += F("',style:{fontFamily:'sans-serif'}},\n"
                         "title:{text:''},"
                         "series:[");
        for (int i = 0; i < axes; i++) {
            *tmpPageBuf += F("{data:[]}");
            if (i != axes - 1) *tmpPageBuf += ',';
        }
        *tmpPageBuf += F("],\n"
                         "xAxis:{type:'datetime',dateTimeLabelFormats:{second:'%H:%M:%S'}},\n"
                         "yAxis:{title:{enabled:false}},"
                         "credits:{enabled:false}});\n"
                         "setInterval(function(){var xhttp=new XMLHttpRequest();var ch=");
        *tmpPageBuf += name;
        *tmpPageBuf += F("\n"
                         "xhttp.onreadystatechange=function(){if(this.readyState==4&&this.status==200){\n"
                         "var x=new Date().getTime()-new Date().getTimezoneOffset()*60000;"
                         "var arr=this.responseText.split(',');"
                         "var move=(ch.series[0].data.length>");
        *tmpPageBuf += xamount;
        *tmpPageBuf += F(");\n"
                         "for(let i=0;i<arr.length;i++)ch.series[i].addPoint([x,Number(arr[i])],true,move,true);\n"
                         "}};xhttp.open('GET','/GP_update?");
        *tmpPageBuf += name;
        *tmpPageBuf += F("=',true);xhttp.send();},\n");
        *tmpPageBuf += prd;
        *tmpPageBuf += F(");</script>\n");
        send();
    }
    void AJAX_PLOT(const String& name, const char** labels, int axes, int xamount = 20, int prd = 1000, int height = 400, bool local = 0) {
        *tmpPageBuf += F("<script src='");

        if (local) *tmpPageBuf += F("/gp_data/AJAX_PLOT.js");
        else *tmpPageBuf += F("https://code.highcharts.com/highcharts.js");

        *tmpPageBuf += F("'></script>\n<div id='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' class='chartBlock' style='height:");
        *tmpPageBuf += height;
        *tmpPageBuf += F("px'></div><script>\n var ");
        *tmpPageBuf += name;
        *tmpPageBuf += F("=new Highcharts.Chart({\n"
                         "chart:{borderRadius:10,renderTo:'");
        *tmpPageBuf += name;
        *tmpPageBuf += F("',style:{fontFamily:'sans-serif'}},\n"
                         "title:{text:''},"
                         "series:[");
        for (int i = 0; i < axes; i++) {
            *tmpPageBuf += F("{data:[],name:'");
            *tmpPageBuf += labels[i];
            *tmpPageBuf += F("'}");
            if (i != axes - 1) *tmpPageBuf += ',';
        }
        *tmpPageBuf += F("],\n"
                         "xAxis:{type:'datetime',dateTimeLabelFormats:{second:'%H:%M:%S'}},\n"
                         "yAxis:{title:{enabled:false}},"
                         "credits:{enabled:false}});\n"
                         "setInterval(function(){var xhttp=new XMLHttpRequest();var ch=");
        *tmpPageBuf += name;
        *tmpPageBuf += F("\n"
                         "xhttp.onreadystatechange=function(){if(this.readyState==4&&this.status==200){\n"
                         "var x=new Date().getTime()-new Date().getTimezoneOffset()*60000;"
                         "var arr=this.responseText.split(',');"
                         "var move=(ch.series[0].data.length>");
        *tmpPageBuf += xamount;
        *tmpPageBuf += F(");\n"
                         "for(let i=0;i<arr.length;i++)ch.series[i].addPoint([x,Number(arr[i])],true,move,true);\n"
                         "}};xhttp.open('GET','/GP_update?");
        *tmpPageBuf += name;
        *tmpPageBuf += F("=',true);xhttp.send();},\n");
        *tmpPageBuf += prd;
        *tmpPageBuf += F(");</script>\n");
        send();
    }

    void AJAX_PLOT_DARK(const String& name, int axes, int xamount = 20, int prd = 1000, int height = 400, bool local = 0) {
        if (local) *tmpPageBuf += F("<script src='/gp_data/AJAX_PLOT.js'></script>\n"
                                    "<script src='/gp_data/AJAX_PLOT_DARK.js'></script>\n");
        else *tmpPageBuf += F("<script src='https://code.highcharts.com/highcharts.js'></script>\n"
                              "<script src='https://code.highcharts.com/themes/dark-unica.js'></script>\n");

        *tmpPageBuf += F("<div id='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' class='chartBlock' style='height:");
        *tmpPageBuf += height;
        *tmpPageBuf += F("px'></div><script>\n var ");
        *tmpPageBuf += name;
        *tmpPageBuf += F("=new Highcharts.Chart({\n"
                         "chart:{borderRadius:10,renderTo:'");
        *tmpPageBuf += name;
        *tmpPageBuf += F("',style:{fontFamily:'sans-serif'}},\n"
                         "title:{text:''},"
                         "series:[");
        for (int i = 0; i < axes; i++) {
            *tmpPageBuf += F("{data:[]}");
            if (i != axes - 1) *tmpPageBuf += ',';
        }
        *tmpPageBuf += F("],\n"
                         "xAxis:{type:'datetime',dateTimeLabelFormats:{second:'%H:%M:%S'}},\n"
                         "yAxis:{title:{enabled:false}},"
                         "credits:{enabled:false}});\n"
                         "setInterval(function(){var xhttp=new XMLHttpRequest();var ch=");
        *tmpPageBuf += name;
        *tmpPageBuf += F("\n"
                         "xhttp.onreadystatechange=function(){if(this.readyState==4&&this.status==200){\n"
                         "var x=new Date().getTime()-new Date().getTimezoneOffset()*60000;"
                         "var arr=this.responseText.split(',');"
                         "var move=(ch.series[0].data.length>");
        *tmpPageBuf += xamount;
        *tmpPageBuf += F(");\n"
                         "for(let i=0;i<arr.length;i++)ch.series[i].addPoint([x,Number(arr[i])],true,move,true);\n"
                         "}};xhttp.open('GET','/GP_update?");
        *tmpPageBuf += name;
        *tmpPageBuf += F("=',true);xhttp.send();},\n");
        *tmpPageBuf += prd;
        *tmpPageBuf += F(");</script>\n");
        send();
    }
    void AJAX_PLOT_DARK(const String& name, const char** labels, int axes, int xamount = 20, int prd = 1000, int height = 400, bool local = 0) {
        if (local) *tmpPageBuf += F("<script src='/gp_data/AJAX_PLOT.js'></script>\n"
                                    "<script src='/gp_data/AJAX_PLOT_DARK.js'></script>\n");
        else *tmpPageBuf += F("<script src='https://code.highcharts.com/highcharts.js'></script>\n"
                              "<script src='https://code.highcharts.com/themes/dark-unica.js'></script>\n");

        *tmpPageBuf += F("<div id='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' class='chartBlock' style='height:");
        *tmpPageBuf += height;
        *tmpPageBuf += F("px'></div><script>\n var ");
        *tmpPageBuf += name;
        *tmpPageBuf += F("=new Highcharts.Chart({\n"
                         "chart:{borderRadius:10,renderTo:'");
        *tmpPageBuf += name;
        *tmpPageBuf += F("',style:{fontFamily:'sans-serif'}},\n"
                         "title:{text:''},"
                         "series:[");
        for (int i = 0; i < axes; i++) {
            *tmpPageBuf += F("{data:[],name:'");
            *tmpPageBuf += labels[i];
            *tmpPageBuf += F("'}");
            if (i != axes - 1) *tmpPageBuf += ',';
        }
        *tmpPageBuf += F("],\n"
                         "xAxis:{type:'datetime',dateTimeLabelFormats:{second:'%H:%M:%S'}},\n"
                         "yAxis:{title:{enabled:false}},"
                         "credits:{enabled:false}});\n"
                         "setInterval(function(){var xhttp=new XMLHttpRequest();var ch=");
        *tmpPageBuf += name;
        *tmpPageBuf += F("\n"
                         "xhttp.onreadystatechange=function(){if(this.readyState==4&&this.status==200){\n"
                         "var x=new Date().getTime()-new Date().getTimezoneOffset()*60000;"
                         "var arr=this.responseText.split(',');"
                         "var move=(ch.series[0].data.length>");
        *tmpPageBuf += xamount;
        *tmpPageBuf += F(");\n"
                         "for(let i=0;i<arr.length;i++)ch.series[i].addPoint([x,Number(arr[i])],true,move,true);\n"
                         "}};xhttp.open('GET','/GP_update?");
        *tmpPageBuf += name;
        *tmpPageBuf += F("=',true);xhttp.send();},\n");
        *tmpPageBuf += prd;
        *tmpPageBuf += F(");</script>\n");
        send();
    }

    template <int ax, int am>
    void PLOT_STOCK(const String& id, const char** labels, uint32_t* times, int16_t vals[ax][am], int dec = 0, int height = 400, bool local = 0) {
        *tmpPageBuf += F("<script src='");

        if (local) *tmpPageBuf += F("/gp_data/PLOT_STOCK.js");
        else *tmpPageBuf += F("https://code.highcharts.com/stock/highstock.js");

        *tmpPageBuf += F("'></script>\n <div class='chartBlock' style='height:");
        *tmpPageBuf += height;
        *tmpPageBuf += F("px' id='");
        *tmpPageBuf += id;
        *tmpPageBuf += F("'></div>");
        *tmpPageBuf += F("<script>Highcharts.stockChart('");
        *tmpPageBuf += id;
        *tmpPageBuf += F("',{chart:{},\n"
                         "rangeSelector:{buttons:[\n"
                         "{count:1,type:'minute',text:'1M'},\n"
                         "{count:1,type:'hour',text:'1H'},\n"
                         "{count:1,type:'day',text:'1D'},\n"
                         "{type:'all',text:'All'}],\n"
                         "inputEnabled:false,selected:0},\n"
                         "time:{useUTC:false},\n"
                         "credits:{enabled:false},series:[\n");
        send();
        for (int axs = 0; axs < ax; axs++) {
            *tmpPageBuf += F("{name:'");
            *tmpPageBuf += labels[axs];
            *tmpPageBuf += F("',data:[\n");
            for (int ams = 0; ams < am; ams++) {
                *tmpPageBuf += '[';
                *tmpPageBuf += times[ams];
                *tmpPageBuf += F("000");
                *tmpPageBuf += ',';
                if (dec) *tmpPageBuf += (float)vals[axs][ams] / dec;
                else *tmpPageBuf += vals[axs][ams];
                *tmpPageBuf += "],\n";
                send();
            }
            *tmpPageBuf += "]},\n";
        }
        *tmpPageBuf += F("]});</script>\n");
        send();
    }

    template <int ax, int am>
    void PLOT_STOCK_DARK(const String& id, const char** labels, uint32_t* times, int16_t vals[ax][am], int dec = 0, int height = 400, bool local = 0) {
        if (local) *tmpPageBuf += F("<script src='/gp_data/PLOT_STOCK.js'></script>\n"
                                    "<script src='/gp_data/PLOT_STOCK_DARK.js'></script>\n");
        else *tmpPageBuf += F("<script src='https://code.highcharts.com/stock/highstock.js'></script>\n"
                              "<script src='https://code.highcharts.com/themes/dark-unica.js'></script>\n");

        *tmpPageBuf += F("<div class='chartBlock' style='height:");
        *tmpPageBuf += height;
        *tmpPageBuf += F("px' id='");
        *tmpPageBuf += id;
        *tmpPageBuf += F("'></div>");
        *tmpPageBuf += F("<script>Highcharts.stockChart('");
        *tmpPageBuf += id;
        *tmpPageBuf += F("',{chart:{},\n"
                         "rangeSelector:{buttons:[\n"
                         "{count:1,type:'minute',text:'1M'},\n"
                         "{count:1,type:'hour',text:'1H'},\n"
                         "{count:1,type:'day',text:'1D'},\n"
                         "{type:'all',text:'All'}],\n"
                         "inputEnabled:false,selected:0},\n"
                         "time:{useUTC:false},\n"
                         "credits:{enabled:false},series:[\n");
        send();
        for (int axs = 0; axs < ax; axs++) {
            *tmpPageBuf += F("{name:'");
            *tmpPageBuf += labels[axs];
            *tmpPageBuf += F("',data:[\n");
            for (int ams = 0; ams < am; ams++) {
                *tmpPageBuf += '[';
                *tmpPageBuf += times[ams];
                *tmpPageBuf += F("000");
                *tmpPageBuf += ',';
                if (dec) *tmpPageBuf += (float)vals[axs][ams] / dec;
                else *tmpPageBuf += vals[axs][ams];
                *tmpPageBuf += "],\n";
                send();
            }
            *tmpPageBuf += "]},\n";
        }
        *tmpPageBuf += F("]});</script>\n");
        send();
    }

    // ================== ОТПРАВКА ОБЪЕКТОВ ===================
    //    template <typename T>
    //    void PUT_OBJ(const T& ar) { Serial.println(__FUNCSIG__); }

    void PUT_OBJ(const GP::AREA& ar) { this->AREA(ar.name, ar.rows, ar.text, ar.width, ar.disabled); }
    void PUT_OBJ(const GP::BUTTON& btn) { this->BUTTON(btn.name, btn.text, btn.target, btn.style, btn.width, btn.disabled, btn.reload); }
    void PUT_OBJ(const GP::BUTTON_MINI& btn) { this->BUTTON_MINI(btn.name, btn.text, btn.target, btn.style, btn.width); }
    void PUT_OBJ(const GP::CHECK& ch) { this->CHECK(ch.name, ch.state, ch.style, ch.disabled); }
    void PUT_OBJ(const GP::COLOR& c) { this->COLOR(c.name, c.color, c.disabled); }
    void PUT_OBJ(const GP::DATE& d) { this->DATE(d.name, d.date, d.disabled); }
    void PUT_OBJ(const GP::LABEL& label) { this->LABEL(label.text, label.name, label.style, label.size, label.bold, label.wrap); }
    void PUT_OBJ(const GP::LABEL_BLOCK& label) { this->LABEL_BLOCK(label.text, label.name, label.style, label.size, label.bold); }
    void PUT_OBJ(const GP::LED& led) { this->LED(led.name, led.state); }
    void PUT_OBJ(const GP::LED_GREEN& led) { this->LED_GREEN(led.name, led.state); }
    void PUT_OBJ(const GP::LED_RED& led) { this->LED_RED(led.name, led.state); }
    void PUT_OBJ(const GP::NUMBER& num) { this->NUMBER_RAW(num.name, num.placeholder, (num.value == INT32_MAX ? String("") : String(num.value)), num.min, num.max, num.width, num.pattern, num.disabled); }
    void PUT_OBJ(const GP::NUMBER_F& num) { this->NUMBER_F(num.name, num.placeholder, num.value, num.decimals, num.width, num.disabled), NUMBER_RAW(num.name, num.placeholder, (isnan(num.value) ? String("") : String(num.value, (uint16_t)num.decimals)), num.min, num.max, num.width, num.pattern, num.disabled); }
    void PUT_OBJ(const GP::PASS& pas) { this->PASS(pas.name, pas.placeholder, pas.text, pas.width, pas.maxlen, pas.pattern, pas.disabled, pas.eye); }
    void PUT_OBJ(const GP::RADIO& r) { this->RADIO(r.name, r.num, r.value, r.style, r.disabled); }
    void PUT_OBJ(const GP::SELECT& s) { this->SELECT(s.name, s.list, s.selected, s.numbers, s.disabled, s.reload); }
    void PUT_OBJ(const GP::SLIDER& s) { this->SLIDER(s.name, s.value, s.min, s.max, s.step, s.decimals, s.style, s.disabled, s.oninput); }
    void PUT_OBJ(const GP::SPINNER& s) { this->SPINNER(s.name, s.value, s.min, s.max, s.step, s.decimals, s.style, s.width, s.disabled); }
    void PUT_OBJ(const GP::SWITCH& sw) { this->SWITCH(sw.name, sw.state, sw.style, sw.disabled); }
    void PUT_OBJ(const GP::TEXT& txt) { this->TEXT(txt.name, txt.placeholder, txt.text, txt.width, txt.maxlen, txt.pattern, txt.disabled); }
    void PUT_OBJ(const GP::TIME& t) { this->TIME(t.name, t.time, t.disabled); }
    void PUT_OBJ(const GP::TITLE& title) { this->TITLE(title.text, title.name, title.style, title.size, title.bold); }
    void PUT_OBJ(const GP::TD&) { this->TD(); }
    void PUT_OBJ(const GP::TR&) { this->TR(); }

    //    void TITLE(GP::TITLE& title) {
    //        TITLE(title.text, title.name, title.styler, title.size, title.bold);
    //    }
    //    void LABEL(GP::LABEL& label) {
    //        LABEL(label.text, label.name, label.style, label.size, label.bold, label.wrap);
    //    }
    //    void LABEL_BLOCK(GP::LABEL_BLOCK& label) {
    //        LABEL_BLOCK(label.text, label.name, label.style, label.size, label.bold);
    //    }

    //    void LED(GP::LED& led) {
    //        LED(led.name, led.state);
    //    }
    //    void LED_RED(GP::LED_RED& led) {
    //        LED_RED(led.name, led.state);
    //    }
    //    void LED_GREEN(GP::LED_GREEN& led) {
    //        LED_GREEN(led.name, led.state);
    //    }

    //    void BUTTON(GP::BUTTON& btn) {
    //        BUTTON(btn.name, btn.text, btn.target, btn.style, btn.width, btn.disabled, btn.reload);
    //    }
    //    void BUTTON_MINI(GP::BUTTON_MINI& btn) {
    //        BUTTON_MINI(btn.name, btn.text, btn.target, btn.style, btn.width);
    //    }

    //    void NUMBER(GP::NUMBER& num) {
    //        NUMBER_RAW(num.name, num.placeholder, (num.value == INT32_MAX ? String("") : String(num.value)), num.min, num.max, num.width, num.pattern, num.disabled);
    //    }
    //    void NUMBER_F(GP::NUMBER_F& num) {
    //        NUMBER_F(num.name, num.placeholder, num.value, num.decimals, num.width, num.disabled);
    //        NUMBER_RAW(num.name, num.placeholder, (isnan(num.value) ? String("") : String(num.value, (uint16_t)num.decimals)), num.min, num.max, num.width, num.pattern, num.disabled);
    //    }

    //    void TEXT(GP::TEXT& txt) {
    //        TEXT(txt.name, txt.placeholder, txt.text, txt.width, txt.maxlen, txt.pattern, txt.disabled);
    //    }
    //    void PASS(GP::PASS& pas) {
    //        PASS(pas.name, pas.placeholder, pas.text, pas.width, pas.maxlen, pas.pattern, pas.disabled, pas.eye);
    //    }

    //    void AREA(GP::AREA& ar) {
    //        AREA(ar.name, ar.rows, ar.text, ar.width, ar.disabled);
    //    }

    //    void CHECK(GP::CHECK& ch) {
    //        CHECK(ch.name, ch.state, ch.style, ch.disabled);
    //    }
    //    void SWITCH(GP::SWITCH& sw) {
    //        SWITCH(sw.name, sw.state, sw.style, sw.disabled);
    //    }

    //    void DATE(GP::DATE& d) {
    //        DATE(d.name, d.date, d.disabled);
    //    }
    //    void TIME(GP::TIME& t) {
    //        TIME(t.name, t.time, t.disabled);
    //    }
    //    void COLOR(GP::COLOR& c) {
    //        COLOR(c.name, c.color, c.disabled);
    //    }

    //    void SPINNER(GP::SPINNER& s) {
    //        SPINNER(s.name, s.value, s.min, s.max, s.step, s.decimals, s.style, s.width, s.disabled);
    //    }
    //    void SLIDER(GP::SLIDER& s) {
    //        SLIDER(s.name, s.value, s.min, s.max, s.step, s.decimals, s.style, s.disabled, s.oninput);
    //    }

    //    void SELECT(GP::SELECT& s) {
    //        SELECT(s.name, s.list, s.selected, s.numbers, s.disabled, s.reload);
    //    }

    //    void RADIO(GP::RADIO& r) {
    //        RADIO(r.name, r.num, r.value, r.style, r.disabled);
    //    }
};

inline Builder GP;

} // namespace GP
