using System.Text;

namespace tests
{
    public class TestBase
    {
        protected static string Multiline(params string[] lines)
        {
            var builder = new StringBuilder();

            foreach (var line in lines)
            {
                builder.AppendLine(line);
            }

            return builder.ToString();
        }
    }
}
