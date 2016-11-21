using System;
using System.Collections.Generic;

namespace dwarf.core
{
    public class BytecodeBuilder
    {
        private List<byte> bytecode = new List<byte>();

        private Dictionary<Label, int> labels = new Dictionary<Label, int>();
        private Dictionary<Label, List<int>> fixups = new Dictionary<Label, List<int>>();

        private int LabelOffset(Label label)
        {
            if (labels.ContainsKey(label))
                return labels[label] - (bytecode.Count + 4);

            if (!fixups.ContainsKey(label))
                fixups.Add(label, new List<int>());

            fixups[label].Add(bytecode.Count);
            return bytecode.Count + 4;
        }

        private void DoFixups(List<int> offsets, int address)
        {
            foreach (var offset in offsets)
            {
                var pc = BitConverter.ToInt32(bytecode.GetRange(offset, 4).ToArray(), 0);
                var b = BitConverter.GetBytes(address - pc);

                for (int i = 0; i < 4; i++)
                {
                    bytecode[offset + i] = b[i];
                }
            }
        }

        public List<byte> Bytecode
        {
            get { return bytecode; }
        }

        public BytecodeBuilder halt()
        {
            bytecode.Add(0x00);
            return this;
        }

        public BytecodeBuilder iadd()
        {
            bytecode.Add(0x01);
            return this;
        }

        public BytecodeBuilder isub()
        {
            bytecode.Add(0x02);
            return this;
        }

        public BytecodeBuilder imul()
        {
            bytecode.Add(0x03);
            return this;
        }

        public BytecodeBuilder idiv()
        {
            bytecode.Add(0x04);
            return this;
        }

        public BytecodeBuilder dup()
        {
            bytecode.Add(0x05);
            return this;
        }

        public BytecodeBuilder ipushc(long c)
        {
            bytecode.Add(0x06);
            bytecode.AddRange(BitConverter.GetBytes(c));
            return this;
        }

        public BytecodeBuilder ipushreg(short reg)
        {
            bytecode.Add(0x07);
            bytecode.AddRange(BitConverter.GetBytes(reg));
            return this;
        }

        public BytecodeBuilder ipopreg(short reg)
        {
            bytecode.Add(0x08);
            bytecode.AddRange(BitConverter.GetBytes(reg));
            return this;
        }

        public BytecodeBuilder jmp(Label label)
        {
            bytecode.Add(0x09);
            bytecode.AddRange(BitConverter.GetBytes(LabelOffset(label)));
            return this;
        }

        public BytecodeBuilder jz(Label label)
        {
            bytecode.Add(0x0A);
            bytecode.AddRange(BitConverter.GetBytes(LabelOffset(label)));
            return this;
        }

        public BytecodeBuilder jnz(Label label)
        {
            bytecode.Add(0x0B);
            bytecode.AddRange(BitConverter.GetBytes(LabelOffset(label)));
            return this;
        }

        public BytecodeBuilder icmp()
        {
            bytecode.Add(0x0C);
            return this;
        }

        public BytecodeBuilder eq()
        {
            bytecode.Add(0x0D);
            return this;
        }

        public BytecodeBuilder print()
        {
            bytecode.Add(0x0E);
            return this;
        }

        public BytecodeBuilder label(Label label)
        {
            if (fixups.ContainsKey(label))
            {
                DoFixups(fixups[label], bytecode.Count);
                fixups.Remove(label);
            }

            labels.Add(label, bytecode.Count);

            return this;
        }
    }
}
