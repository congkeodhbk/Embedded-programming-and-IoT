using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Framework
{
    public class Slip
    {
        enum State
        {
            SLIP_STATE_DECODING, //!< Ready to receive the next byte.
            SLIP_STATE_ESC_RECEIVED, //!< An ESC byte has been received and the next byte must be decoded differently.
            SLIP_STATE_CLEARING_INVALID_PACKET
        };
        const byte SLIP_BYTE_END = 192;    /* indicates end of packet */
        const byte SLIP_BYTE_ESC = 219;    /* indicates byte stuffing */
        const byte SLIP_BYTE_ESC_END = 220;   /* ESC ESC_END means END data byte */
        const byte SLIP_BYTE_ESC_ESC = 221;    /* ESC ESC_ESC means ESC data byte */

        List<byte> buffer;
        State state = State.SLIP_STATE_DECODING;
        public Slip()
        {
            buffer = new List<byte>();
        }

        public byte[] encodeByte(byte b)
        {
            List<byte> output = new List<byte>();
            switch (b)
            { 
                case SLIP_BYTE_END:
                    output.Add(SLIP_BYTE_ESC);
                    output.Add(SLIP_BYTE_ESC_END);
                break;

                case SLIP_BYTE_ESC:
                    output.Add(SLIP_BYTE_ESC);
                    output.Add(SLIP_BYTE_ESC_ESC);
                break;

                default:
                    output.Add(b);
                break;
            }
            return output.ToArray();
        }
        public byte[] encode(byte[] input)
        {
            List<byte> output = new List<byte>();
            output.Add(SLIP_BYTE_END);
            foreach (byte b in input)
            {
                output.AddRange(encodeByte(b));
            }
            output.Add(SLIP_BYTE_END);
            return output.ToArray();
        }

        public void encode(List<byte> input)
        {
            input.Insert(0, SLIP_BYTE_END);
            for(int i=1; i<input.Count; i++)
            {
                switch (input[i])
                {
                    case SLIP_BYTE_END:
                        input.RemoveAt(i);
                        input.Insert(i, SLIP_BYTE_ESC_END);
                        input.Insert(i, SLIP_BYTE_ESC);
                        break;

                    case SLIP_BYTE_ESC:
                        input.Insert(i+1, SLIP_BYTE_ESC_ESC);
                        break;

                    default:
                        break;
                }
            }
            input.Add(SLIP_BYTE_END);
        }
        public  byte[] encode(byte cmd, string text)
        {
            List<byte> output = new List<byte>();
            byte [] textByte=Encoding.ASCII.GetBytes(text);
            output.AddRange(encodeByte(cmd));
            foreach(byte b in textByte)
            {
                output.AddRange(encodeByte(b));
            }
            output.Add(0); //string null terminator
            output.Add(SLIP_BYTE_END);
            return output.ToArray();
        }

        public byte[] encode(byte cmd, byte[] data)
        {
            List<byte> output = new List<byte>();
            output.AddRange(encodeByte(cmd));
            foreach (byte b in data)
            {
                output.AddRange(encodeByte(b));
            }
            output.Add(SLIP_BYTE_END);
            return output.ToArray();
        }

        public List<byte> parse(byte c)
        {
            switch (state)
            {
                case State.SLIP_STATE_DECODING:
                    switch (c)
                    {
                        case SLIP_BYTE_END:
                            state = State.SLIP_STATE_DECODING;
                            if (buffer.Count > 0)
                            {
                                List<byte> result = buffer;
                                buffer = new List<byte>();
                                return result;
                            }
                            else
                            {
                                return null;
                            }
                        case SLIP_BYTE_ESC:
                            // wait for
                            state = State.SLIP_STATE_ESC_RECEIVED;
                            break;
                        default:
                            buffer.Add(c);
                            break;
                    }
                    break;
                case State.SLIP_STATE_ESC_RECEIVED:
                    switch (c)
                    {
                        case SLIP_BYTE_ESC_END:
                            buffer.Add(SLIP_BYTE_END);
                            state = State.SLIP_STATE_DECODING;
                            break;

                        case SLIP_BYTE_ESC_ESC:
                            buffer.Add(SLIP_BYTE_ESC);
                            state = State.SLIP_STATE_DECODING;
                            break;

                        default:
                            // protocol violation
                            state = State.SLIP_STATE_CLEARING_INVALID_PACKET;
                            return null;
                    }
                    break;
                case State.SLIP_STATE_CLEARING_INVALID_PACKET:
                    if (c == SLIP_BYTE_END)
                    {
                        state = State.SLIP_STATE_DECODING;
                        buffer.Clear();
                    }
                    break;
            }
            return null;
        }
    }
}
