using System.IO;
using dwarf.core;
using NUnit.Framework;

namespace tests
{
    [TestFixture]
    public class VmTests
    {
        [Test]
        public void Test1()
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

        [Test]
        public void Test2()
        {
            var builder = new BytecodeBuilder();

            builder
                .ipushc(1)
                .dup()
                .isub()
                .print()
                .halt();

            var printer = new StringWriter();
            var vm = new Vm(builder.Bytecode, printer);

            vm.Run();

            Assert.AreEqual("0\r\n", printer.ToString());
        }

        [Test]
        public void Test3()
        {
            var builder = new BytecodeBuilder();

            builder
                .ipushc(3)
                .ipushc(5)
                .imul()
                .print()
                .halt();

            var printer = new StringWriter();
            var vm = new Vm(builder.Bytecode, printer);

            vm.Run();

            Assert.AreEqual("15\r\n", printer.ToString());
        }
    }
}
