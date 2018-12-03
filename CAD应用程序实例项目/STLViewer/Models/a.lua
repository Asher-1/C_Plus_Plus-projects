-- Lua script.
p=tetview:new()
p:load_stl("G:/develop/c++Projects/STLViewer/Models/rthas the Lich King1.stl")
rnd=glvCreate(0, 0, 500, 500, "TetView")
p:plot(rnd)
glvWait()
