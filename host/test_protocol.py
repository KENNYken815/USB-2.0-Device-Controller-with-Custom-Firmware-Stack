import unittest
from usb_host import encode, decode, CMD
class ProtocolTest(unittest.TestCase):
    def test_round_trip(self):
        p=encode(CMD['echo'],99,b'test')
        c,s,d=decode(p)
        self.assertEqual((c,s,d),(CMD['echo'],99,b'test'))
    def test_corruption(self):
        p=bytearray(encode(CMD['ping'],1))
        p[12:12]=b'X'
        with self.assertRaises(ValueError):
            decode(bytes(p))
if __name__=='__main__':
    unittest.main()
