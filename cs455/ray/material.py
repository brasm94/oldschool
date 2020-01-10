import numpy as np
class material:
    def __init__(self,diff,spec,phong):
        self.diffuse = np.asarray(diff)
        self.spec_high = np.asarray(spec)
        self.phong_con = np.asarray(phong)
    def get_diff(self):
        return self.diffuse
    def get_spec(self):
        return self.spec_high
    def get_phong(self):
        return self.phong_con