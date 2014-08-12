using System;
using System.Collections.Generic;

namespace dwarf.core
{
    public class BytecodeBuilder
    {
        private List<byte> bytecode = new List<byte>();

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

        public BytecodeBuilder print()
        {
            bytecode.Add(0x0E);
            return this;
        }
    }
}
