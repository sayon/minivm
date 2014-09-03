using System;
using dwarf.core.lang;
using NUnit.Framework;

namespace tests
{
    [TestFixture]
    public class LexerTests : TestBase
    {
        [Test]
        public void ExpressionsTest()
        {
            var source = Multiline(
                "a + b",
                "a - b",
                "a / b",
                "a * b");

            var lexer = new Lexer();
            var actual = String.Join("", lexer.Tokenize(source));

            const string expected = "[a] + [b] " +
                                    "[a] - [b] " +
                                    "[a] / [b] " +
                                    "[a] * [b] ";

            Assert.AreEqual(expected, actual);
        }

        [Test]
        public void IfThenElseTest()
        {
            var source = Multiline(
                "if (a == b) then {",
                "    a := a - b;",
                "} else {",
                "    a := 0;",
                "}");

            var lexer = new Lexer();
            var actual = String.Join("", lexer.Tokenize(source));

            const string expected = "if ([a] == [b]) then { " +
                                    "[a] := [a] - [b]; " +
                                    "} else { " +
                                    "[a] := <0>; " +
                                    "} ";

            Assert.AreEqual(expected, actual);
        }

        [Test]
        public void ConstTest()
        {
            var source = Multiline(
                "a := 10",
                "a := -12",
                "a := +13"
                );

            var lexer = new Lexer();
            var actual = String.Join("", lexer.Tokenize(source));

            const string expected = "[a] := <10> " +
                                    "[a] := <-12> " +
                                    "[a] := <13> ";

            Assert.AreEqual(expected, actual);
        }
    }
}
