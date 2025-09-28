SUBDIRS += Source
SUBDIRS += Sink
# SUBDIRS += decoder

.PHONY: $(SUBDIRS)

all: $(SUBDIRS)
	@set -e; for dir in $(SUBDIRS); do $(MAKE) -C $${dir} $@; done

clean:
	@set -e; for dir in $(SUBDIRS); do $(MAKE) -C $${dir} $@; done
	@rm -f omni* rtc.conf .confFilePath
