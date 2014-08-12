using System.IO;
using System.Text;
using dwarf.core;
using NUnit.Framework;

namespace tests
{
    [TestFixture]
    public class VmTests
    {
        private static string Multiline(params string[] lines)
        {
            var builder = new StringBuilder();

            foreach (var line in lines)
            {
                builder.AppendLine(line);
            }

            return builder.ToString();
        }

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

            Assert.AreEqual(Multiline("3"), printer.ToString());
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

            Assert.AreEqual(Multiline("0"), printer.ToString());
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

            Assert.AreEqual(Multiline("15"), printer.ToString());
        }

        [Test]
        public void Test4()
        {
            var builder = new BytecodeBuilder();

            var l1 = new Label();

            builder
                .ipushc(1)
                .jz(l1)
                .ipushc(10)
                .print()

              .label(l1)
                .ipushc(20)
                .print()
                .halt();

            var printer = new StringWriter();
            var vm = new Vm(builder.Bytecode, printer);

            vm.Run();

            Assert.AreEqual(Multiline("10", "20"), printer.ToString());
        }

        [Test]
        public void Test5()
        {
            var builder = new BytecodeBuilder();

            var l1 = new Label();
            var l2 = new Label();
            var l3 = new Label();

            builder
                .jmp(l2)

              .label(l1)
                .ipushc(10)
                .print()
                .jmp(l3)

              .label(l2)
                .jmp(l1)

              .label(l3)
                .halt();

            var printer = new StringWriter();
            var vm = new Vm(builder.Bytecode, printer);

            vm.Run();

            Assert.AreEqual(Multiline("10"), printer.ToString());
        }

        [Test]
        public void Test6()
        {
            var builder = new BytecodeBuilder();

            builder
                .ipushc(10)
                .ipushc(10)
                .eq()
                .print()

                .ipushc(10)
                .ipushc(11)
                .eq()
                .print()

                .halt();

            var printer = new StringWriter();
            var vm = new Vm(builder.Bytecode, printer);

            vm.Run();

            Assert.AreEqual(Multiline("1", "0"), printer.ToString());
        }

        [Test]
        public void Test7()
        {
            var builder = new BytecodeBuilder();

            var loop = new Label();

            builder
                .ipushc(10)

              .label(loop)
                .dup()
                .print()

                .ipushc(1)
                .isub()
                .dup()
                .jnz(loop)

                .halt();

            var printer = new StringWriter();
            var vm = new Vm(builder.Bytecode, printer);

            vm.Run();

            Assert.AreEqual(Multiline("10", "9", "8", "7", "6", "5", "4", "3", "2", "1"), printer.ToString());
        }
    }
}
