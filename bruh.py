myfile = open("/home/omerfaruk/Projects/okul/BOB/div.cpp", "a")
lines = []
#lines.append("ap_uint<5> LUT(ap_uint<16> input) {\nap_uint<5> bruh; switch(input) {\ncase 0:\nbruh = 16;\nbreak;\n")
for j in range(0, 8):
    for i in range(2**j,2**(j+1)):
        lines.append(str(7-j) + ", ")
    # lines.append("bruh="+str(15-j)+";\nbreak;\n")
#lines.append("default:\n bruh = 0;}}")
myfile.writelines(lines)
myfile.close()
