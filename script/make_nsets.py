#
# *WIP* Example script to write 'nsets.conf'
#

outfile = open("nsets.conf","wb")

numEs = 2

v=-10.0
m = 0.0
n = 0.0
h = 0.0
ms = 0.0
mhf = 0.0
mhs = 0.0
mtc = 0.0
htc = 0.0
mck = 0.0
ca = 0.0
gna = 52.0
ena = 55.0
gk = 11.0
ek = -90.0
gl = .55
el = -65.0
iext = -2.5450#-1.8#-2.545

for i in range(numEs):	
	outfile.write("v:%f,m:%f,n:%f,h:%f,ms:%f,mhf:%f,mhs:%f,mtc:%f,htc:%f,mck:%f,ca:%f,gna:%f,ena:%f,gk:%f,ek:%f,gl:%f,el:%f,iext:%f\n"%(v,m,n,h,ms,mhf,mhs,mtc,htc,mck,ca,gna,ena,gk,ek,gl,el,iext))

outfile.write("v:-58,m:0.0,n:0.0,h:0.0,iext:1.0\n")
outfile.close()
