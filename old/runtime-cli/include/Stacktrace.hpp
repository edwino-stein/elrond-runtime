#if !defined  _ELROND_STACKTRACE_STANDALONE_HPP
    #define _ELROND_STACKTRACE_STANDALONE_HPP

    #include "elrond-runtime.hpp"

    class Stacktrace {

        public:

            struct StackSymbol {
                void* addr;
                elrond::sizeT id;
                elrond::String backtrace;
                elrond::String name;
                elrond::String offset;
                elrond::String path;

                StackSymbol(void* addr, elrond::sizeT id, elrond::String backtrace);
                StackSymbol(const StackSymbol& s);
            };

            using eachSymbolCallbackT = ELROND_LAMBDA_FUNC(void, StackSymbol&);
            using SymbolCollectionT = std::vector<StackSymbol>;

            static bool each(eachSymbolCallbackT handle,
                             const elrond::sizeT depth,
                             const elrond::sizeT skip = 1);

            static void dump(std::ostream& os,
                             const elrond::sizeT skip,
                             const elrond::sizeT depth = 63);

            static void dump(SymbolCollectionT& collection,
                             const elrond::sizeT skip,
                             const elrond::sizeT depth = 63);

            static void dump(std::ostream& os, const SymbolCollectionT& collection);

        private:

            Stacktrace();

            static void parseSymbol(StackSymbol& symbol);

            static void parseName(StackSymbol& symbol,
                                  const elrond::sizeT beginName,
                                  const elrond::sizeT beginOffset);

            static void parseOffset(StackSymbol& symbol,
                                    const elrond::sizeT beginOffset,
                                    const elrond::sizeT endOffset);

            static void parsePath(StackSymbol& symbol, const elrond::sizeT beginName);
    };

#endif
