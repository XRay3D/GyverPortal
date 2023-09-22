#pragma once

// GP Macro

#include "builder.h"
#include "objects.h"
#include "utils.h"

namespace GP {

struct Flag { };

template <typename Ty, typename... Ts>
struct Printable : std::tuple<Ts...>, Flag {
    static constexpr size_t Size = sizeof...(Ts);
    using tuple = std::tuple<Ts...>;
    using tuple::tuple;
    //    enum {
    //        Size = sizeof...(Ts)
    //    };
    //    Printable(Printable&&) = delete;
    //    Printable(const Printable&) = delete;
    void build() const {
        print_impl(*static_cast<const Ty*>(this), std::make_index_sequence<Size>{});
    }

    mutable int i{};

private:
    template <typename _Ty, size_t... Is>
    /*static*/ void print_impl(const _Ty& t, std::index_sequence<Is...>) const {
        if constexpr (std::is_base_of_v<Flag, _Ty>) {
            t.begin();
            ([this](auto&& val) {
                using T = std::decay_t<decltype(val)>;
                if constexpr (std::is_base_of_v<Flag, T>)
                    print_impl(val, std::make_index_sequence<T::Size>{});
                else
                    print_impl(val, std::index_sequence<>{});
            }(std::get<Is>(t)),
                ...);
            t.end();
        } else {
            static_cast<const Ty*>(this)->begin2();
            if constexpr (std::is_base_of_v<HasText, _Ty>) {
                if (t.text.endsWith("#")) {
                    auto copy{t};
                    copy.text += i;
                    GP.PUT_OBJ(copy);
                } else {
                    GP.PUT_OBJ(t);
                }
            } else
                GP.PUT_OBJ(t);
            static_cast<const Ty*>(this)->end2();
        }
    }
};

template <typename Ty, typename... Ts>
Printable(Ty, Ts...) -> Printable<Ty, Ts...>;

/// \brief The BOX class
template <typename... Ts>
struct BOX : Printable<BOX<Ts...>, Ts...> {
    using P = Printable<BOX<Ts...>, Ts...>;
    BOX(Ts&&... ts)
        : P{std::forward<Ts>(ts)...} { }
    void begin() const { GP.BOX_BEGIN(); }
    void begin2() const { }
    void end() const { GP.BOX_END(); }
    void end2() const { }
};

template <typename... Ts>
BOX(Ts&&...) -> BOX<Ts...>;

/// \brief The GRID class
template <typename... Ts>
struct GRID : Printable<GRID<Ts...>, Ts...> {
    using P = Printable<GRID<Ts...>, Ts...>;
    GRID(Ts&&... ts)
        : P{std::forward<Ts>(ts)...} { }
    void begin() const { GP.GRID_BEGIN(); }
    void begin2() const { }
    void end() const { GP.GRID_END(); }
    void end2() const { }
};

template <typename... Ts>
GRID(Ts&&...) -> GRID<Ts...>;

/// \brief The BLOCK_THIN_TAB class
template <typename... Ts>
struct BLOCK_THIN_TAB : Printable<BLOCK_THIN_TAB<Ts...>, Ts...> {
    using P = Printable<BLOCK_THIN_TAB<Ts...>, Ts...>;
    String name;
    BLOCK_THIN_TAB(const String& name, Ts&&... ts)
        : P{std::forward<Ts>(ts)...}
        , name{name} { }
    void begin() const { GP.BLOCK_THIN_TAB_BEGIN(name); }
    void begin2() const { }
    void end() const { GP.BLOCK_END(); }
    void end2() const { }
};

template <typename... Ts>
BLOCK_THIN_TAB(const String&, Ts&&...) -> BLOCK_THIN_TAB<Ts...>;

/// \brief The BLOCK_THIN_TAB class
template <typename... Ts>
struct M_TR : Printable<M_TR<Ts...>, Ts...> {
    using P = Printable<M_TR<Ts...>, Ts...>;
    String name;
    M_TR(const String& name, Ts&&... ts)
        : P{std::forward<Ts>(ts)...}
        , name{name} { }
    M_TR(Ts&&... ts)
        : P{std::forward<Ts>(ts)...} { }

    void begin() const { GP.TR(); }
    void begin2() { GP.TD(); }
    void end() const { }
    void end2() const { }
};

template <typename... Ts>
M_TR(const String&, Ts&&...) -> M_TR<Ts...>;
template <typename... Ts>
M_TR(Ts&&...) -> M_TR<Ts...>;

/// \brief The BLOCK_THIN_TAB class
template <size_t N, typename... Ts>
struct M_TABLE : Printable<M_TABLE<N, Ts...>, Ts...> {
    using P = Printable<M_TABLE<N, Ts...>, Ts...>;
    String name;
    std::array<Align, N> align;

    M_TABLE(const String& name, Ts&&... ts)
        : P{std::forward<Ts>(ts)...}
        , name{name} { }

    M_TABLE(Ts&&... ts)
        : P{std::forward<Ts>(ts)...} { }

    M_TABLE(const String& name, const std::array<Align, N>& align, Ts&&... ts)
        : P{std::forward<Ts>(ts)...}
        , name{name}
        , align{align} { }

    M_TABLE(const std::array<Align, N>& align, Ts&&... ts)
        : P{std::forward<Ts>(ts)...}
        , align{align} { }

    void begin() const { GP.TABLE_BEGIN(name, N ? const_cast<Align*>(align.data()) : nullptr); }
    void begin2() const { GP.TD(); }
    void end() const { GP.TABLE_END(); }
    void end2() const { }
};

template <size_t N, typename... Ts>
M_TABLE(const String&, std::array<Align, N>, Ts&&...) -> M_TABLE<N, Ts...>;
template <size_t N, typename... Ts>
M_TABLE(const char*, std::array<Align, N>, Ts&&...) -> M_TABLE<N, Ts...>;
template <size_t N, typename... Ts>
M_TABLE(std::array<Align, N>, Ts&&...) -> M_TABLE<N, Ts...>;

template <typename... Ts>
M_TABLE(const String&, Ts&&...) -> M_TABLE<0, Ts...>;
template <typename... Ts>
M_TABLE(const char*, Ts&&...) -> M_TABLE<0, Ts...>;
template <typename... Ts>
M_TABLE(Ts&&...) -> M_TABLE<0, Ts...>;

// template <size_t N, typename... Ts>
// M_TABLE(const String&, Align (&)[N], Ts&&...) -> M_TABLE<N, Ts...>;
// template <size_t N, typename... Ts>
// M_TABLE(const char*, Align (&)[N], Ts&&...) -> M_TABLE<N, Ts...>;
// template <size_t N, typename... Ts>
// M_TABLE(Align (&)[N], Ts&&...) -> M_TABLE<N, Ts...>;

// inline struct {
//     int i{};
//     operator int() { return i; }
//     auto& operator=(int i_) { return i = i_, *this; }
//     auto& operator++() { return ++i, *this; }
// } I;

template <typename... Ts>
struct FOR : Printable<FOR<Ts...>, Ts...> {
    using P = Printable<FOR<Ts...>, Ts...>;
    int count{};
    FOR(int count, Ts&&... ts)
        : P{std::forward<Ts>(ts)...}
        , count{count} { }
    void begin() const {
        qDebug() << P::i;
        if (P::i == count) P::i = 0;
    }
    void begin2() const { GP.TD(); }
    void end() const {
        qDebug() << P::i;
        if (++P::i == count) return;
        P::build();
    }
    void end2() const { }
};
template <typename... Ts>
FOR(int, Ts&&...) -> FOR<Ts...>;

} // namespace GP

#if 0
// https://stackoverflow.com/a/30566098
#define OVR_MACRO(M, ...)                                      _OVR(M, _COUNT_ARGS(__VA_ARGS__))(__VA_ARGS__)
#define _OVR(mName, nArgs)                                     _OVR_EXPAND(mName, nArgs)
#define _OVR_EXPAND(mName, nArgs)                              mName##nArgs
#define _COUNT_ARGS(...)                                       _ARG_MATCH(__VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1)
#define _ARG_MATCH(_1, _2, _3, _4, _5, _6, _7, _8, _9, N, ...) N

/*
// example
#define foo(...) OVR_MACRO(foo, __VA_ARGS__)
#define foo1(a) bar(a);
#define foo2(a, b) bar(a, b);
#define foo3(a, b, c) bar(a, b, c);
// ...
*/

#define GP_ALS(...) __extension__({static GP::Align __als[] = {__VA_ARGS__}; __als; })

#define M_FORM(...) OVR_MACRO(M_FORM, __VA_ARGS__)
#define M_FORM2(act, args)  \
    GP::GP.FORM_BEGIN(act); \
    args;                   \
    GP::GP.FORM_END();
#define M_FORM3(act, subm, args) \
    GP::GP.FORM_BEGIN(act);      \
    args;                        \
    GP::GP.SUBMIT(subm);         \
    GP::GP.FORM_END();

#define M_GRID(...) OVR_MACRO(M_GRID, __VA_ARGS__)
#define M_GRID1(args)    \
    GP::GP.GRID_BEGIN(); \
    args;                \
    GP::GP.GRID_END();
#define M_GRID2(w, args)  \
    GP::GP.GRID_BEGIN(w); \
    args;                 \
    GP::GP.GRID_END();

#define M_TABLE(...) OVR_MACRO(M_TABLE, __VA_ARGS__)
#define M_TABLE1(args)    \
    GP::GP.TABLE_BEGIN(); \
    args;                 \
    GP::GP.TABLE_END();
#define M_TABLE2(tdw, args)  \
    GP::GP.TABLE_BEGIN(tdw); \
    args;                    \
    GP::GP.TABLE_END();
#define M_TABLE3(tdw, als, args)  \
    GP::GP.TABLE_BEGIN(tdw, als); \
    args;                         \
    GP::GP.TABLE_END();
#define M_TABLE4(tdw, als, w, args)  \
    GP::GP.TABLE_BEGIN(tdw, als, w); \
    args;                            \
    GP::GP.TABLE_END();

#define M_TD(...) OVR_MACRO(M_TD, __VA_ARGS__)
#define M_TD1(args) \
    GP::GP.TD();    \
    args;
#define M_TD2(al, args) \
    GP::GP.TD(w, al);   \
    args;
#define M_TD3(al, cs, args) \
    GP::GP.TD(w, al, cs);   \
    args;
#define M_TD4(al, cs, rs, args) \
    GP::GP.TD(w, al, cs, rs);   \
    args;

#define M_TR(...) OVR_MACRO(M_TR, __VA_ARGS__)
#define M_TR1(a1) \
    GP::GP.TR();  \
    GP::GP.TD();  \
    a1;
#define M_TR2(a1, a2) \
    M_TR1(a1);        \
    GP::GP.TD();      \
    a2;
#define M_TR3(a1, a2, a3) \
    M_TR2(a1, a2);        \
    GP::GP.TD();          \
    a3;
#define M_TR4(a1, a2, a3, a4) \
    M_TR3(a1, a2, a3);        \
    GP::GP.TD();              \
    a4;
#define M_TR5(a1, a2, a3, a4, a5) \
    M_TR4(a1, a2, a3, a4);        \
    GP::GP.TD();                  \
    a5;
#define M_TR6(a1, a2, a3, a4, a5, a6) \
    M_TR5(a1, a2, a3, a4, a5);        \
    GP::GP.TD();                      \
    a6;
#define M_TR7(a1, a2, a3, a4, a5, a6, a7) \
    M_TR6(a1, a2, a3, a4, a5, a6);        \
    GP::GP.TD();                          \
    a7;
#define M_TR8(a1, a2, a3, a4, a5, a6, a7, a8) \
    M_TR7(a1, a2, a3, a4, a5, a6, a7);        \
    GP::GP.TD();                              \
    a8;
#define M_TR9(a1, a2, a3, a4, a5, a6, a7, a8, a9) \
    M_TR8(a1, a2, a3, a4, a5, a6, a7, a8);        \
    GP::GP.TD();                                  \
    a9;
#define M_TR10(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) \
    M_TR9(a1, a2, a3, a4, a5, a6, a7, a8, a9);          \
    GP::GP.TD();                                        \
    a10;

#define M_BOX(...) OVR_MACRO(M_BOX, __VA_ARGS__)
#define M_BOX1(args)    \
    GP::GP.BOX_BEGIN(); \
    args;               \
    GP::GP.BOX_END();
#define M_BOX2(align, args)  \
    GP::GP.BOX_BEGIN(align); \
    args;                    \
    GP::GP.BOX_END();
#define M_BOX3(align, width, args)  \
    GP::GP.BOX_BEGIN(align, width); \
    args;                           \
    GP::GP.BOX_END();
#define M_BOX4(align, width, top, args)  \
    GP::GP.BOX_BEGIN(align, width, top); \
    args;                                \
    GP::GP.BOX_END();

#define M_BLOCK(...) OVR_MACRO(M_BLOCK, __VA_ARGS__)
#define M_BLOCK1(args)    \
    GP::GP.BLOCK_BEGIN(); \
    args;                 \
    GP::GP.BLOCK_END();
#define M_BLOCK2(type, args)  \
    GP::GP.BLOCK_BEGIN(type); \
    args;                     \
    GP::GP.BLOCK_END();
#define M_BLOCK3(type, width, args)  \
    GP::GP.BLOCK_BEGIN(type, width); \
    args;                            \
    GP::GP.BLOCK_END();
#define M_BLOCK4(type, width, text, args)  \
    GP::GP.BLOCK_BEGIN(type, width, text); \
    args;                                  \
    GP::GP.BLOCK_END();
#define M_BLOCK5(type, width, text, style, args)  \
    GP::GP.BLOCK_BEGIN(type, width, text, style); \
    args;                                         \
    GP::GP.BLOCK_END();

#define M_JQ_UPDATE(...) OVR_MACRO(M_JQ_UPDATE, __VA_ARGS__)
#define M_JQ_UPDATE1(args)    \
    GP::GP.JQ_UPDATE_BEGIN(); \
    args;                     \
    GP::GP.JQ_UPDATE_END();
#define M_JQ_UPDATE2(prd, args)  \
    GP::GP.JQ_UPDATE_BEGIN(prd); \
    args;                        \
    GP::GP.JQ_UPDATE_END();
#define M_JQ_UPDATE3(prd, del, args)  \
    GP::GP.JQ_UPDATE_BEGIN(prd, del); \
    args;                             \
    GP::GP.JQ_UPDATE_END();

#define M_NAV_BLOCK(...) OVR_MACRO(M_NAV_BLOCK, __VA_ARGS__)
#define M_NAV_BLOCK1(args)    \
    GP::GP.NAV_BLOCK_BEGIN(); \
    args;                     \
    GP::GP.NAV_BLOCK_END();
#define M_NAV_BLOCK3(name, pos, args)  \
    GP::GP.NAV_BLOCK_BEGIN(name, pos); \
    args;                              \
    GP::GP.NAV_BLOCK_END();

#define M_SPOILER(...) OVR_MACRO(M_SPOILER, __VA_ARGS__)
#define M_SPOILER2(txt, args)  \
    GP::GP.SPOILER_BEGIN(txt); \
    args;                      \
    GP::GP.SPOILER_END();
#define M_SPOILER3(txt, style, args)  \
    GP::GP.SPOILER_BEGIN(txt, style); \
    args;                             \
    GP::GP.SPOILER_END();

// legacy
#define M_BLOCK_TAB(...) OVR_MACRO(M_BLOCK_TAB, __VA_ARGS__)
#define M_BLOCK_TAB2(text, args)  \
    GP::GP.BLOCK_TAB_BEGIN(text); \
    args;                         \
    GP::GP.BLOCK_END();
#define M_BLOCK_TAB3(text, width, args)  \
    GP::GP.BLOCK_TAB_BEGIN(text, width); \
    args;                                \
    GP::GP.BLOCK_END();
#define M_BLOCK_TAB4(text, width, style, args)  \
    GP::GP.BLOCK_TAB_BEGIN(text, width, style); \
    args;                                       \
    GP::GP.BLOCK_END();

#define M_BLOCK_THIN(...) OVR_MACRO(M_BLOCK_THIN, __VA_ARGS__)
#define M_BLOCK_THIN1(args)    \
    GP::GP.BLOCK_THIN_BEGIN(); \
    args;                      \
    GP::GP.BLOCK_END();
#define M_BLOCK_THIN2(width, args)  \
    GP::GP.BLOCK_THIN_BEGIN(width); \
    args;                           \
    GP::GP.BLOCK_END();

#define M_BLOCK_THIN_TAB(...) OVR_MACRO(M_BLOCK_THIN_TAB, __VA_ARGS__)
#define M_BLOCK_THIN_TAB2(text, args)  \
    GP::GP.BLOCK_THIN_TAB_BEGIN(text); \
    args;                              \
    GP::GP.BLOCK_END();
#define M_BLOCK_THIN_TAB3(text, width, args)  \
    GP::GP.BLOCK_THIN_TAB_BEGIN(text, width); \
    args;                                     \
    GP::GP.BLOCK_END();

#define GP_MAKE_FORM(...)           M_FORM(__VA_ARGS__)
#define GP_MAKE_GRID(...)           M_GRID(__VA_ARGS__)
#define GP_MAKE_TABLE(...)          M_TABLE(__VA_ARGS__)
#define GP_MAKE_TD(...)             M_TD(__VA_ARGS__)
#define GP_MAKE_TR(...)             M_TR(__VA_ARGS__)
#define GP_MAKE_BOX(...)            M_BOX(__VA_ARGS__)
#define GP_MAKE_BLOCK(...)          M_BLOCK(__VA_ARGS__)
#define GP_MAKE_BLOCK_TAB(...)      M_BLOCK_TAB(__VA_ARGS__)
#define GP_MAKE_BLOCK_THIN(...)     M_BLOCK_THIN(__VA_ARGS__)
#define GP_MAKE_BLOCK_THIN_TAB(...) M_BLOCK_THIN_TAB(__VA_ARGS__)
#define GP_MAKE_JQ_UPDATE(...)      M_JQ_UPDATE(__VA_ARGS__)
#define GP_MAKE_NAV_BLOCK(...)      M_NAV_BLOCK(__VA_ARGS__)
#define GP_MAKE_SPOILER(...)        M_SPOILER(__VA_ARGS__)

#endif
