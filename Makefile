soc:
	(cd ./client/; make)
	cp ./client/client ./soc
tidy:
	(cd ./background/; make clean)
	(cd ./client/; make clean)
	(cd ./common/; make clean)
	@rm -f *~ *.dat
.PHONY: clean
clean: tidy
	@rm -f soc
