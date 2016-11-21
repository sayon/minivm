namespace dwarf.core.lang
{
    public class Token
    {
    }

    public class KeywordToken : Token
    {
        private string keyword;

        public KeywordToken(string keyword)
        {
            this.keyword = keyword;
        }

        public override string ToString()
        {
            return keyword;
        }
    }

    public class IdentifierToken : Token
    {
        private string name;

        public IdentifierToken(string name)
        {
            this.name = name;
        }

        public override string ToString()
        {
            return string.Format("[{0}]", name);
        }
    }

    public class ConstantToken : Token
    {
        private long value;

        public ConstantToken(long value)
        {
            this.value = value;
        }

        public override string ToString()
        {
            return string.Format("<{0}>", value);
        }
    }

    public class WhitespaceToken : Token
    {
        public override string ToString()
        {
            return " ";
        }
    }
}
