from PIL import Image
import numpy as np

class Img2Bitmap:

    resolution: int
    image: Image.Image
    digit_w_shift: int

    def __init__(self, image_path, resolution=16):
        self.image = Image.open(image_path)
        self.resolution = resolution
        self.digit_w_shift = self.image.size[1]/resolution

    def getRawDigitBitmap(self, digit=0):
        bm = np.zeros((self.resolution, self.resolution))

        w, h = self.image.size
        for y in range(0, self.resolution):
            for x in range(0, self.resolution):
                pos = (x*self.digit_w_shift +
                       self.image.size[1]*digit, y*self.digit_w_shift)
                bm[x, y] = self.image.getpixel(pos)[0] <= 128

        return bm

    def encodeDigitBitmap(self, bm: np.ndarray):
        a = np.zeros((bm.shape[1], 2), dtype=np.uint8)

        for i in range(bm.shape[1]):
            l, h = (0, 0)
            l = np.sum(2**np.array(range(8)) * bm[i,:8])
            h = np.sum(2**np.array(range(8)) * bm[i,8:])
            a[i] = (l,h)
        
        return a

    def printEncodedDigitBitmap(self, a: np.ndarray):
        print("{")
        for i in range(a.shape[0]):
            bl = str(bin(a[i][0]))
            bh = str(bin(a[i][1]))

            bl = "0b"+"0"*(10-len(bl))+bl[2:]
            bh = "0b"+"0"*(10-len(bh))+bh[2:]

            print(bh, ",", bl, ",")
        print("}")
    
    def generateEncodedDigitBitmap(self, digit=0):
        self.printEncodedDigitBitmap(self.encodeDigitBitmap(self.getRawDigitBitmap(digit)))


if __name__ == "__main__":
    i2b = Img2Bitmap("image.png")
    for i in range(10):
        i2b.generateEncodedDigitBitmap(i)
