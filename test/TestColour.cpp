#include <string>
#include <iostream>
#include <gtest/gtest.h>
#include <FFF/fff.h>
DEFINE_FFF_GLOBALS;

#include "bricli/bricli.h"

// Setup fake functions
FAKE_VALUE_FUNC(int, BspWrite, uint32_t, const char*);
FAKE_VALUE_FUNC(int, Test_Handler, uint32_t, char **);

static char _stringHistory[50][80] = {0};
static uint8_t _stringHistoryCount = 0;

int CustomBspWriteFake(uint32_t length, const char* data)
{
	printf("%s", data);

	memset(&_stringHistory[_stringHistoryCount][0], 0, 80);
	memcpy(&_stringHistory[_stringHistoryCount][0], data, length);
	_stringHistoryCount++;

	return BspWrite_fake.return_val;
}

namespace Cli
{
    class ColourTest : public ::testing::Test
    {
    protected:
        BricliCommand_t _commandList[2] =
        {
            { "test", Test_Handler, "Tests.", BricliScopeAll },
            BRICLI_COMMAND_LIST_TERMINATOR
        };
        BricliHandle_t _cli;
        char _buffer[100] = {0};

        ColourTest() { }

        virtual ~ColourTest() { }

        virtual void SetUp() override
        {
            // Reset fake functions.
            RESET_FAKE(BspWrite);
            RESET_FAKE(Test_Handler);
            FFF_RESET_HISTORY();

            // Reset string history
            memset(&_stringHistory[0][0], 0, (50 * 80));
            _stringHistoryCount = 0;

            // Pre-load return values for the fakes.
            BspWrite_fake.return_val = (int)BricliOk;
            BspWrite_fake.custom_fake = CustomBspWriteFake;
            Test_Handler_fake.return_val = (int)BricliOk;

            // Configure our default BriCLI settings.
            BricliInit_t init = {0};
            init.CommandList = _commandList;
            init.RxBuffer = _buffer;
            init.RxBufferSize = 100;
            init.BspWrite = BspWrite;
            init.Settings.EnableColour = true;

            Bricli_Init(&_cli, &init);
        }

        virtual void TearDown() override 
        {
            Bricli_ClearBuffer(&_cli);
         }

    };

    TEST_F(ColourTest, WriteColoured)
    {
        std::string testCommand("Some Response Data");

        Bricli_WriteColoured(&_cli, testCommand.length(), (char *)testCommand.c_str(), BricliTextRed);
        EXPECT_EQ(testCommand.length(), BspWrite_fake.arg0_history[1]);
        EXPECT_STREQ("\e[0;31m", BspWrite_fake.arg1_history[0]);
        EXPECT_STREQ(testCommand.c_str(), BspWrite_fake.arg1_history[1]);
        EXPECT_STREQ("\e[0m", BspWrite_fake.arg1_history[2]);
    }

    TEST_F(ColourTest, WriteColouredLine)
    {
        std::string testCommand("Some Response Data");

        Bricli_WriteColouredLine(&_cli, testCommand.length(), (char *)testCommand.c_str(), BricliTextRed);
        EXPECT_EQ(testCommand.length(), BspWrite_fake.arg0_history[1]);
        EXPECT_STREQ("\e[0;31m", BspWrite_fake.arg1_history[0]);
        EXPECT_STREQ(testCommand.c_str(), BspWrite_fake.arg1_history[1]);
        EXPECT_STREQ(_cli.Eol, BspWrite_fake.arg1_history[2]);
        EXPECT_STREQ("\e[0m", BspWrite_fake.arg1_history[3]);
    }

    TEST_F(ColourTest, WriteStringColoured)
    {
        std::string testCommand("Some Response Data");

        Bricli_WriteStringColoured(&_cli, (char *)testCommand.c_str(), BricliTextRed);
        EXPECT_EQ(testCommand.length(), BspWrite_fake.arg0_history[1]);
        EXPECT_STREQ("\e[0;31m", BspWrite_fake.arg1_history[0]);
        EXPECT_STREQ(testCommand.c_str(), BspWrite_fake.arg1_history[1]);
        EXPECT_STREQ("\e[0m", BspWrite_fake.arg1_history[2]);
    }

    TEST_F(ColourTest, WriteStringColouredLine)
    {
        std::string testCommand("Some Response Data");

        Bricli_WriteStringColouredLine(&_cli, (char *)testCommand.c_str(), BricliTextRed);
        EXPECT_EQ(testCommand.length(), BspWrite_fake.arg0_history[1]);
        EXPECT_STREQ("\e[0;31m", BspWrite_fake.arg1_history[0]);
        EXPECT_STREQ(testCommand.c_str(), BspWrite_fake.arg1_history[1]);
        EXPECT_STREQ(_cli.Eol, BspWrite_fake.arg1_history[2]);
        EXPECT_STREQ("\e[0m", BspWrite_fake.arg1_history[3]);
    }

    TEST_F(ColourTest, PrintFColoured)
    {
        std::string testCommand("Some Response Data");

        BRICLI_PRINTF_COLOURED(&_cli, BricliTextYellow, "%s", testCommand.c_str());
        EXPECT_EQ(testCommand.length(), strlen(&_stringHistory[1][0]));
        EXPECT_STREQ("\e[0;33m", &_stringHistory[0][0]);
        EXPECT_STREQ(testCommand.c_str(), &_stringHistory[1][0]);
        EXPECT_STREQ("\e[0m", &_stringHistory[2][0]);
    }

}