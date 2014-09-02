using System;
using System.Collections.Generic;
using System.Linq;

namespace dwarf.core.lang
{
    interface ITokenRule
    {
        bool Check(char c, int pos);
        Token CreateToken(string src);
    }

    internal class IdentifierRule : ITokenRule
    {
        public bool Check(char c, int pos)
        {
            return pos == 0 ? Char.IsLetter(c) : Char.IsLetterOrDigit(c);
        }

        public Token CreateToken(string src)
        {
            return new IdentifierToken(src);
        }
    }

    internal class KeywordRule : ITokenRule
    {
        private string keyword;

        public KeywordRule(string keyword)
        {
            this.keyword = keyword;
        }

        public bool Check(char c, int pos)
        {
            return pos < keyword.Length && keyword[pos] == c;
        }

        public Token CreateToken(string src)
        {
            return new KeywordToken(src);
        }
    }

    internal class NumberRule : ITokenRule
    {
        public bool Check(char c, int pos)
        {
            if (pos == 0 && (c == '-' && c == '+'))
                return true;
            else
                return Char.IsNumber(c);
        }

        public Token CreateToken(string src)
        {
            return new ConstantToken(long.Parse(src));
        }
    }

    internal class WhitespaceRule : ITokenRule
    {
        public bool Check(char c, int pos)
        {
            return Char.IsWhiteSpace(c);
        }

        public Token CreateToken(string src)
        {
            return new WhitespaceToken();
        }
    }

    internal class Matcher
    {
        private ITokenRule rule;
        private bool stop;
        private string matched;

        public Token Token { get; private set; }

        public Matcher(ITokenRule rule)
        {
            this.rule = rule;
        }

        public void Reset()
        {
            stop = false;
            matched = String.Empty;
            Token = null;
        }

        public void Update(char c)
        {
            if (stop)
                return;

            if (rule.Check(c, matched.Length))
            {
                matched += c;
            }
            else
            {
                stop = true;
            }

            if (stop && matched.Length > 0)
            {
                Token = rule.CreateToken(matched);
            }
        }
    }

    public class Lexer
    {
        private static readonly ITokenRule[] Rules =
        {
            new WhitespaceRule(),

            new KeywordRule("if"),
            new KeywordRule("then"),
            new KeywordRule("else"),

            new KeywordRule("+"),
            new KeywordRule("-"),
            new KeywordRule("*"),
            new KeywordRule("/"),

            new KeywordRule("while"),

            new KeywordRule("{"),
            new KeywordRule("}"),

            new KeywordRule("("),
            new KeywordRule(")"),

            new KeywordRule("=="),

            new KeywordRule(";"),

            new KeywordRule(":="),

            new IdentifierRule(),
            new NumberRule(),
        };

        public IEnumerable<Token> Tokenize(string source)
        {
            var matchers = Rules.Select(rule => new Matcher(rule)).ToArray();

            var state = 0;
            var pos = 0;

            // HACK
            source += (char) 0xffff;

            while (pos < source.Length)
            {
                if (state == 0)
                {
                    foreach (var matcher in matchers)
                    {
                        matcher.Reset();
                    }

                    state = 1;
                }

                foreach (var matcher in matchers)
                {
                    matcher.Update(source[pos]);
                }

                Token token = null;
                foreach (var matcher in matchers)
                {
                    if (matcher.Token != null)
                    {
                        token = matcher.Token;
                        break;
                    }
                }

                if (token != null)
                {
                    state = 0;
                    yield return token;
                }
                else
                {
                    pos ++;
                }
            }
        }
    }
}
