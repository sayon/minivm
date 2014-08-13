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
    }
}
