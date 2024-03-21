connect: 
	ssh student_3@alex-freenas.ddns.net -p2000
send:
	scp -P 2000 *.c parametr.h himenoDVMH.cdv student_3@alex-freenas.ddns.net:/home/student_3/himeno
clear:
	rm *.o
compile_init:
	gcc -o himenoInit himenoInit.c
compile_oacc:
	ompcc -o himenoOACC himenoOACC.c
compile_xmp:
	xmpcc -o himenoXMP himenoXMP.c
compile_xacc:
	xmpcc -xacc -o himenoXACC himenoXACC.c


