.PHONY: clean all 

all:
	$(MAKE) -C commun
	$(MAKE) -C client
	$(MAKE) -C serveur
	$(MAKE) -C test

clean:
	$(MAKE) clean -C client
	$(MAKE) clean -C serveur
	$(MAKE) clean -C commun
	$(MAKE) clean -C test
	rm -f gmon.out