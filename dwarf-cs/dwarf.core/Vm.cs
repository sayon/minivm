using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace dwarf.core
{
    public class Vm
    {
        private byte[] bytecode;
        private readonly StringWriter printer;

        private Stack<long> stack = new Stack<long>();
        private bool halted;
        private int pc;

        public Vm(IEnumerable<byte> bytecode, StringWriter printer)
        {
            this.bytecode = bytecode.ToArray();
            this.printer = printer;
        }

        public void Run()
        {
            pc = 0;
            halted = false;

            while (!halted)
            {
                var instruction = bytecode[pc++];

                switch (instruction)
                {
                    case 0x00:
                        halted = true;
                        break;

                    case 0x01:
                        stack.Push(stack.Pop() + stack.Pop());
                        break;

                    case 0x02:
                    {
                        var b = stack.Pop();
                        var a = stack.Pop();

                        stack.Push(a - b);
                        break;
                    }

                    case 0x03:
                        stack.Push(stack.Pop() * stack.Pop());
                        break;

                    case 0x05:
                        stack.Push(stack.Peek());
                        break;

                    case 0x06:
                        stack.Push(BitConverter.ToInt64(bytecode, pc));
                        pc += sizeof(long);
                        break;

                    case 0x09:
                    {
                        var off = BitConverter.ToInt32(bytecode, pc);
                        pc += sizeof (int);

                        pc = pc + off;
                        break;
                    }

                    case 0x0A:
                    {
                        var off = BitConverter.ToInt32(bytecode, pc);
                        pc += sizeof (int);

                        if (stack.Pop() == 0)
                            pc = pc + off;
                        break;
                    }

                    case 0x0B:
                    {
                        var off = BitConverter.ToInt32(bytecode, pc);
                        pc += sizeof(int);

                        if (stack.Pop() != 0)
                            pc = pc + off;
                        break;
                    }

                    case 0x0D:
                    {
                        var a = stack.Pop();
                        var b = stack.Pop();

                        stack.Push(a == b ? 1 : 0);

                        break;
                    }

                    case 0x0E:
                        printer.WriteLine(stack.Pop());
                        break;
                }
            }
        }
    }
}
