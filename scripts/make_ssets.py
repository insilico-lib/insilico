#
# *WIP* Example script to write 'ssets.conf'
# 

outfile = open("ssets.conf","wb")

numEs = 2

g1=0.222
g2=0.222

e_gsyn=10.0
e_tau1=.01
e_tau2=.01
e_esyn=20

i_gsyn= 100.0
i_tau1=.20
i_tau2=.20
i_esyn=-100.0

las_spik=0.0

for i in range(numEs):
	outfile.write("g1:%f,g2:%f,gsyn:%f,tau1:%f,tau2:%f,esyn:%f,pre:%d,post:%d,las_spik:%f\n"%(g1,g2,e_gsyn,e_tau1,e_tau2,e_esyn,i,numEs,las_spik))
for i in range(numEs):
	outfile.write("g1:%f,g2:%f,gsyn:%f,tau1:%f,tau2:%f,esyn:%f,pre:%d,post:%d,las_spik:%f\n"%(g1,g2,(i+1)*i_gsyn,i_tau1,i_tau2,i_esyn,numEs,i,las_spik))
outfile.close()
