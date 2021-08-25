cmd_Release/obj.target/main.node := g++ -o Release/obj.target/main.node -shared -pthread -rdynamic -m64  -Wl,-soname=main.node -Wl,--start-group Release/obj.target/main/main.o -Wl,--end-group 
