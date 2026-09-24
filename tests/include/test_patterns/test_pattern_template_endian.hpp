#pragma once

#include "test_pattern.hpp"

namespace pl::test {

    class TestPatternTemplateEndian : public TestPattern {
    public:
        TestPatternTemplateEndian(core::Evaluator *evaluator) : TestPattern(evaluator, "TemplateEndian") {
        }
        ~TestPatternTemplateEndian() override = default;

        [[nodiscard]] std::string getSourceCode() const override {
            return R"(
                struct EqBox<V> {
                    V v;
                };
                struct EqPayload {
                    u32 x;
                };
                struct EqBodyBe<V> {
                    be V v;
                };
                struct EqStructPayload {
                    be u32 x;
                };

                be u32 eqPlainBe @ 0;
                std::assert(eqPlainBe == 0x89504E47, "sanity: plain 'be u32' must read big endian");

                u32 eqPlainDefault @ 0;
                std::assert(eqPlainDefault == 0x474E5089, "sanity: default 'u32' must read little endian");

                EqBox<be u32> eqBoxBe @ 0;
                std::assert(eqBoxBe.v == 0x89504E47, "'be' on a type template argument must be preserved");

                EqBox<be EqPayload> eqBoxCustomBe @ 0;
                std::assert(eqBoxCustomBe.v.x == 0x89504E47, "'be' on a custom type template argument must be preserved");

                EqBox<EqBox<be u32>> eqBoxNestedBe @ 0;
                std::assert(eqBoxNestedBe.v.v == 0x89504E47, "'be' in a nested template argument must be preserved");

                using EqBoxAlias = EqBox<be u32>;
                EqBoxAlias eqAliasBe @ 0;
                std::assert(eqAliasBe.v == 0x89504E47, "'be' in an alias template argument must be preserved");

                using EqBoxAliasTemplate<V> = EqBox<V>;
                EqBoxAliasTemplate<be u32> eqAliasTemplateBe @ 0;
                std::assert(eqAliasTemplateBe.v == 0x89504E47, "'be' passed through an alias template must be preserved");

                using EqIdentity<V> = V;
                EqIdentity<be u32> eqIdentityBe @ 0;
                std::assert(eqIdentityBe == 0x89504E47, "'be' passed through an identity alias must be preserved");

                EqBox<be u32> eqBoxBeArray[2] @ 0;
                std::assert(eqBoxBeArray[0].v == 0x89504E47, "'be' in an array of template instantiations must be preserved");
                std::assert(eqBoxBeArray[1].v == 0x0D0A1A0A, "'be' in an array of template instantiations must be preserved");

                be EqBox<u32> eqOuterBe @ 0;
                std::assert(eqOuterBe.v == 0x89504E47, "'be' on the outside of a template instantiation must still apply");

                EqBodyBe<u32> eqBodyBe @ 0;
                std::assert(eqBodyBe.v == 0x89504E47, "'be' applied to a template parameter inside the body must apply");

                le EqBox<be u32> eqInnerBeWins @ 4;
                std::assert(eqInnerBeWins.v == 0x0D0A1A0A, "endian of the template argument must win over the outer default");

                EqBox<EqStructPayload> eqStructArg @ 0;
                std::assert(eqStructArg.v.x == 0x89504E47, "a struct with 'be' members must stay big endian when used as a template argument");
            )";
        }
    };

}
