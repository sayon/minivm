using dwarf.core;
using NUnit.Framework;

namespace tests
{
    [TestFixture]
    public class BytecodeBuilderTests
    {
        [Test]
        public void HaltTest()
        {
            var builder = new BytecodeBuilder();

            builder
                .halt();

            var expexted = new byte[]
            {
                0x00 // HALT
            };

            Assert.AreEqual(expexted, builder.Bytecode);
        }

        [Test]
        public void OperationsTest()
        {
            var builder = new BytecodeBuilder();

            builder
                .iadd()
                .isub()
                .imul()
                .idiv();

            var expexted = new byte[]
            {
                0x01, // IADD
                0x02, // ISUB
                0x03, // IMUL
                0x04  // IDIV
            };

            Assert.AreEqual(expexted, builder.Bytecode);
        }

        [Test]
        public void ConstTest()
        {
            var builder = new BytecodeBuilder();

            builder
                .ipushc(0x0102030405060708);

            var expexted = new byte[]
            {
                0x06,
                0x08, 0x07, 0x06, 0x05,
                0x04, 0x03, 0x02, 0x01  // IPUSHC 0x0102030405060708
            };

            Assert.AreEqual(expexted, builder.Bytecode);
        }

        [Test]
        public void JmpTest()
        {
            var builder = new BytecodeBuilder();

            var loop = new Label();
            var next = new Label();

            builder
                .label(loop)
                .jmp(loop)
                .jmp(next)
                .label(next);

            var expexted = new byte[]
            {
                0x09, 0xFB, 0xFF, 0xFF, 0xFF, // JMP -5
                0x09, 0x00, 0x00, 0x00, 0x00  // JMP 0
            };

            Assert.AreEqual(expexted, builder.Bytecode);
        }

        [Test]
        public void JzTest()
        {
            var builder = new BytecodeBuilder();

            var loop = new Label();
            var next = new Label();

            builder
                .label(loop)
                .jz(loop)
                .jz(next)
                .label(next);

            var expexted = new byte[]
            {
                0x0A, 0xFB, 0xFF, 0xFF, 0xFF, // JZ -5
                0x0A, 0x00, 0x00, 0x00, 0x00  // JZ 0
            };

            Assert.AreEqual(expexted, builder.Bytecode);
        }

        [Test]
        public void JnzTest()
        {
            var builder = new BytecodeBuilder();

            var loop = new Label();
            var next = new Label();

            builder
                .label(loop)
                .jnz(loop)
                .jnz(next)
                .label(next);

            var expexted = new byte[]
            {
                0x0B, 0xFB, 0xFF, 0xFF, 0xFF, // JNZ -5
                0x0B, 0x00, 0x00, 0x00, 0x00  // JNZ 0
            };

            Assert.AreEqual(expexted, builder.Bytecode);
        }

        [Test]
        public void PushPopRegTest()
        {
            var builder = new BytecodeBuilder();

            builder
                .ipushreg(0x01)
                .ipopreg(0x02);

            var expexted = new byte[]
            {
                0x07, 0x01, 0x00, // IPUSHREG 1
                0x08, 0x02, 0x00  // IPOPREG 2
            };

            Assert.AreEqual(expexted, builder.Bytecode);
        }

        [Test]
        public void IcmpEqTest()
        {
            var builder = new BytecodeBuilder();

            builder
                .icmp()
                .eq();

            var expexted = new byte[]
            {
                0x0C, // ICMP
                0x0D  // EQ
            };

            Assert.AreEqual(expexted, builder.Bytecode);
        }

        [Test]
        public void PrintTest()
        {
            var builder = new BytecodeBuilder();

            builder
                .print();

            var expexted = new byte[]
            {
                0x0E // PRINT
            };

            Assert.AreEqual(expexted, builder.Bytecode);
        }
    }
}
