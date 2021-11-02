"""
Author: Wyatt Geckle
Date: 11/2/21

Generates G&M code. Writing G&M code the standard way doesn't allow for some useful function
calls, such as sine and cosine. It also doesn't allow for iteration.  Since there wasn't
an ellipse command I could find, I took it upon myself to draw one without writing every
point manually.
"""

class GandMFile:

    def __init__(self, filename):
        self.curr_line = 0
        
        self.f = open(filename, 'w')

    def generate_line(self, command, comment=""):
        
        if comment:
            comment = "\t; {}".format(comment)
        
        self.f.write("N{} {}{}\n".format(self.curr_line, command, comment))
        
        self.curr_line += 1
        
    def close(self):
        self.f.close()
