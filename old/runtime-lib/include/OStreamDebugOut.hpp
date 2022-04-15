#if !defined  _ELROND_RUNTIME_OSTREAM_DEBUG_OUT_HPP
    #define _ELROND_RUNTIME_OSTREAM_DEBUG_OUT_HPP

    #include "rtTypes.hpp"

    namespace elrond {
        namespace runtime {

            class OStreamDebugOut : public elrond::interface::DebugOut {

                protected:

                    std::ostream& os;

                public:

                    OStreamDebugOut(std::ostream& os);

                    const elrond::interface::DebugOut& put(const char c[]) const  override;
                    const elrond::interface::DebugOut& put(char c) const  override;
                    const elrond::interface::DebugOut& put(unsigned char c) const  override;
                    const elrond::interface::DebugOut& put(int i) const  override;
                    const elrond::interface::DebugOut& put(unsigned int i) const  override;
                    const elrond::interface::DebugOut& put(long l) const  override;
                    const elrond::interface::DebugOut& put(unsigned long l) const  override;
                    const elrond::interface::DebugOut& put(double d) const  override;
                    const elrond::interface::DebugOut& put(elrond::String str) const override;

                    const elrond::interface::DebugOut& putLn(const char c[]) const override;
                    const elrond::interface::DebugOut& putLn(char c) const override;
                    const elrond::interface::DebugOut& putLn(unsigned char c) const override;
                    const elrond::interface::DebugOut& putLn(int i) const override;
                    const elrond::interface::DebugOut& putLn(unsigned int i) const override;
                    const elrond::interface::DebugOut& putLn(long l) const override;
                    const elrond::interface::DebugOut& putLn(unsigned long l) const override;
                    const elrond::interface::DebugOut& putLn(double d) const override;
                    const elrond::interface::DebugOut& putLn(void) const override;
                    const elrond::interface::DebugOut& putLn(elrond::String str) const override;

                    const elrond::interface::DebugOut& flush() const override;
            };
        }
    }


#endif
