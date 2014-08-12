using System.IO;
using dwarf.core;
using NUnit.Framework;

namespace tests
{
    [TestFixture]
    public class VmTests
    {
        [Test]
        public void BasicTest()
        {
            var builder = new BytecodeBuilder();

            builder
                .ipushc(1)
                .ipushc(2)
                .iadd()
                .print()
                .halt();

            var printer = new StringWriter();
            var vm = new Vm(builder.Bytecode, printer);

            vm.Run();

            Assert.AreEqual("3\r\n", printer.ToString());
        }
    }
}
