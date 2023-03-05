    The program consists in creating an image editor using commands
in the terminal (e.g. edit, draw, fill, set draw_color, set line_width, etc.)
    For this, I first used a vector in which I read the commands. And then I
called the function corresponding to each command.
    To load the image into memory, I used a function (editfile) of the type
struct gbr (gbr is another structure with the 3 RGB colors) in which I read
the information of the image. Then I saved the pixels in matrix. For the other 
tasks, I also used functions that I have sent the data read from the editfile 
function plus the additional ones for every task.
     To insert the image from task 2, I created an insert function, in which
I read the image that I inserted over the other one.
     For task 3, I determined the coordinates that should join the
the two points using the formula. I set the size of the line by traversing the point
from top to bottom using 2 for statements.
     For task 4, I tried to go through the matrix first, the upper left part
and then I called another function that did the same thing in the upper right part
and then the lower parts.


