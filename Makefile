# CFLAGS += -DSCORE -DCAPTURE -DCLOAKING

all:
	cc $(CFLAGS) -o sst sst.c setup.c reports.c planets.c osx.c moving.c finish.c events.c battle.c ai.c

clean:
	@rm -f sst sst.core
