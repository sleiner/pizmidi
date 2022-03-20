#include <gtest/gtest.h>

/// Adding this listener will abort test execution as soon as an assertion fails
///
/// This class was taken from the googletest docs:
/// https://github.com/google/googletest/blob/3d81736c973fbcc938267cb296918bc3266dde12/docs/advanced.md
class ThrowOnAssertionFailure : public ::testing::EmptyTestEventListener
{
    void OnTestPartResult (const testing::TestPartResult& result) override
    {
        if (result.type() == testing::TestPartResult::kFatalFailure)
        {
            throw testing::AssertionException (result);
        }
    }
};

int main (int argc, char** argv)
{
    ::testing::InitGoogleTest (&argc, argv);
    ::testing::UnitTest::GetInstance()->listeners().Append (new ThrowOnAssertionFailure);
    return RUN_ALL_TESTS();
}
