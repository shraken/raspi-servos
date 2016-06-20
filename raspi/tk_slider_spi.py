import Tkinter as tk
import spi

class App:
    def __init__(self):
        self.root = tk.Tk()
        self._job = None
        self.spi_device = spi.openSPI()
        
        slider_horiz_text = tk.Label(self.root, text="Horizontal")
        slider_horiz_text.pack()
        self.slider_horiz = tk.Scale(self.root, from_=0, to=180,
                                     orient="horizontal",
                                     command=self.updateValueHoriz)
        self.slider_horiz.pack()
        
        slider_vertical_text = tk.Label(self.root, text="Vertical")
        slider_vertical_text.pack()
        self.slider_vert = tk.Scale(self.root, from_=0, to=180,
									orient="horizontal",
									command=self.updateValueVert)
        self.slider_vert.pack()
        self.root.mainloop()

    def updateValueHoriz(self, event):
        if self._job:
            self.root.after_cancel(self._job)
        self._job = self.root.after(10, self._send_spi_string)

    def updateValueVert(self, event):
        if self._job:
            self.root.after_cancel(self._job)
        self._job = self.root.after(10, self._send_spi_string)

    def _send_spi_string(self):
        self._job = None
        send_str = "0:%d:1:%d:\n" % (self.slider_horiz.get(), self.slider_vert.get())
        print send_str
        
        l = tuple(map(ord, send_str))
        print l
        spi.transfer((l))
    
app = App()
