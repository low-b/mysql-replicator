#COMAKE2 edit-mode: -*- Makefile -*-
####################64Bit Mode####################
ifeq ($(shell uname -m),x86_64)
CC=gcc
CXX=g++
CXXFLAGS=-std=c++11 \
  -g \
  -pipe \
  -W \
  -Wall \
  -fPIC
CFLAGS=-std=c++11 \
  -g \
  -pipe \
  -W \
  -Wall \
  -fPIC
CPPFLAGS=-D_GNU_SOURCE \
  -D__STDC_LIMIT_MACROS \
  -DVERSION=\"1.9.8.7\"
INCPATH=-I. \
  -I./include \
  -I./output \
  -I./output/include \
  -I./proto
DEP_INCPATH=-I../../third-64/boost \
  -I../../third-64/boost/include \
  -I../../third-64/boost/output \
  -I../../third-64/boost/output/include \
  -I../../third-64/gtest \
  -I../../third-64/gtest/include \
  -I../../third-64/gtest/output \
  -I../../third-64/gtest/output/include \
  -I../../third-64/protobuf \
  -I../../third-64/protobuf/include \
  -I../../third-64/protobuf/output \
  -I../../third-64/protobuf/output/include

#============ CCP vars ============
CCHECK=@ccheck.py
CCHECK_FLAGS=
PCLINT=@pclint
PCLINT_FLAGS=
CCP=@ccp.py
CCP_FLAGS=


#COMAKE UUID
COMAKE_MD5=135f55c545dd04589ddb0d1367fe8d34  COMAKE


.PHONY:all
all:comake2_makefile_check binlog_listener 
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mall[0m']"
	@echo "make all done"

.PHONY:comake2_makefile_check
comake2_makefile_check:
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mcomake2_makefile_check[0m']"
	#in case of error, update 'Makefile' by 'comake2'
	@echo "$(COMAKE_MD5)">comake2.md5
	@md5sum -c --status comake2.md5
	@rm -f comake2.md5

.PHONY:ccpclean
ccpclean:
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mccpclean[0m']"
	@echo "make ccpclean done"

.PHONY:clean
clean:ccpclean
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mclean[0m']"
	rm -rf binlog_listener
	rm -rf ./output/bin/binlog_listener
	rm -rf src/binlog_listener_bytes_helper.o
	rm -rf src/binlog_listener_db_meta.o
	rm -rf src/binlog_listener_eof_packet.o
	rm -rf src/binlog_listener_err_packet.o
	rm -rf src/binlog_listener_handshake_init_packet.o
	rm -rf src/binlog_listener_handshake_response_packet.o
	rm -rf src/binlog_listener_mysql_replicator_connector.o
	rm -rf src/binlog_listener_mysql_replicator_resultset.o
	rm -rf src/binlog_listener_mysql_replicator_statement.o
	rm -rf src/binlog_listener_ok_packet.o
	rm -rf src/binlog_listener_packet_header.o
	rm -rf src/binlog_listener_query_packet.o
	rm -rf src/binlog_listener_query_response_packet.o

.PHONY:dist
dist:
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mdist[0m']"
	tar czvf output.tar.gz output
	@echo "make dist done"

.PHONY:distclean
distclean:clean
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mdistclean[0m']"
	rm -f output.tar.gz
	@echo "make distclean done"

.PHONY:love
love:
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mlove[0m']"
	@echo "make love done"

binlog_listener:src/binlog_listener_bytes_helper.o \
  src/binlog_listener_db_meta.o \
  src/binlog_listener_eof_packet.o \
  src/binlog_listener_err_packet.o \
  src/binlog_listener_handshake_init_packet.o \
  src/binlog_listener_handshake_response_packet.o \
  src/binlog_listener_mysql_replicator_connector.o \
  src/binlog_listener_mysql_replicator_resultset.o \
  src/binlog_listener_mysql_replicator_statement.o \
  src/binlog_listener_ok_packet.o \
  src/binlog_listener_packet_header.o \
  src/binlog_listener_query_packet.o \
  src/binlog_listener_query_response_packet.o
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mbinlog_listener[0m']"
	$(CXX) src/binlog_listener_bytes_helper.o \
  src/binlog_listener_db_meta.o \
  src/binlog_listener_eof_packet.o \
  src/binlog_listener_err_packet.o \
  src/binlog_listener_handshake_init_packet.o \
  src/binlog_listener_handshake_response_packet.o \
  src/binlog_listener_mysql_replicator_connector.o \
  src/binlog_listener_mysql_replicator_resultset.o \
  src/binlog_listener_mysql_replicator_statement.o \
  src/binlog_listener_ok_packet.o \
  src/binlog_listener_packet_header.o \
  src/binlog_listener_query_packet.o \
  src/binlog_listener_query_response_packet.o -Xlinker "-(" ../../third-64/boost/lib/libboost_atomic.a \
  ../../third-64/boost/lib/libboost_chrono.a \
  ../../third-64/boost/lib/libboost_container.a \
  ../../third-64/boost/lib/libboost_context.a \
  ../../third-64/boost/lib/libboost_coroutine.a \
  ../../third-64/boost/lib/libboost_date_time.a \
  ../../third-64/boost/lib/libboost_exception.a \
  ../../third-64/boost/lib/libboost_filesystem.a \
  ../../third-64/boost/lib/libboost_graph.a \
  ../../third-64/boost/lib/libboost_locale.a \
  ../../third-64/boost/lib/libboost_log_setup.a \
  ../../third-64/boost/lib/libboost_math_c99.a \
  ../../third-64/boost/lib/libboost_math_c99f.a \
  ../../third-64/boost/lib/libboost_math_c99l.a \
  ../../third-64/boost/lib/libboost_math_tr1.a \
  ../../third-64/boost/lib/libboost_math_tr1f.a \
  ../../third-64/boost/lib/libboost_math_tr1l.a \
  ../../third-64/boost/lib/libboost_prg_exec_monitor.a \
  ../../third-64/boost/lib/libboost_program_options.a \
  ../../third-64/boost/lib/libboost_python.a \
  ../../third-64/boost/lib/libboost_random.a \
  ../../third-64/boost/lib/libboost_regex.a \
  ../../third-64/boost/lib/libboost_serialization.a \
  ../../third-64/boost/lib/libboost_signals.a \
  ../../third-64/boost/lib/libboost_system.a \
  ../../third-64/boost/lib/libboost_test_exec_monitor.a \
  ../../third-64/boost/lib/libboost_thread.a \
  ../../third-64/boost/lib/libboost_timer.a \
  ../../third-64/boost/lib/libboost_unit_test_framework.a \
  ../../third-64/boost/lib/libboost_wave.a \
  ../../third-64/boost/lib/libboost_wserialization.a \
  ../../third-64/gtest/lib/libgtest.a \
  ../../third-64/gtest/lib/libgtest_main.a \
  ../../third-64/protobuf/lib/libprotobuf-lite.a \
  ../../third-64/protobuf/lib/libprotobuf.a \
  ../../third-64/protobuf/lib/libprotoc.a -lpthread \
  -lcrypto \
  -lrt \
  -lcrypt -Xlinker "-)" -o binlog_listener
	mkdir -p ./output/bin
	cp -f --link binlog_listener ./output/bin

src/binlog_listener_bytes_helper.o:src/bytes_helper.cpp \
  include/bytes_helper.h \
  include/packet.h \
  include/network_exception.h \
  include/myexception.h \
  include/packet_header.h
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40msrc/binlog_listener_bytes_helper.o[0m']"
	$(CXX) -c $(INCPATH) $(DEP_INCPATH) $(CPPFLAGS) $(CXXFLAGS)  -o src/binlog_listener_bytes_helper.o src/bytes_helper.cpp

src/binlog_listener_db_meta.o:src/db_meta.cpp \
  include/db_meta.h \
  include/mysql_replicator_com.h \
  include/mysql_replicator_connector.h \
  include/mysql_replicator_statement.h \
  include/mysql_replicator_resultset.h \
  include/mysql_replicator_statement.h \
  include/mysql_replicator_resultset.h \
  include/global_constants.h
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40msrc/binlog_listener_db_meta.o[0m']"
	$(CXX) -c $(INCPATH) $(DEP_INCPATH) $(CPPFLAGS) $(CXXFLAGS)  -o src/binlog_listener_db_meta.o src/db_meta.cpp

src/binlog_listener_eof_packet.o:src/eof_packet.cpp \
  include/eof_packet.h \
  include/packet.h \
  include/network_exception.h \
  include/myexception.h \
  include/bytes_helper.h
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40msrc/binlog_listener_eof_packet.o[0m']"
	$(CXX) -c $(INCPATH) $(DEP_INCPATH) $(CPPFLAGS) $(CXXFLAGS)  -o src/binlog_listener_eof_packet.o src/eof_packet.cpp

src/binlog_listener_err_packet.o:src/err_packet.cpp \
  include/err_packet.h \
  include/packet.h \
  include/network_exception.h \
  include/myexception.h \
  include/bytes_helper.h
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40msrc/binlog_listener_err_packet.o[0m']"
	$(CXX) -c $(INCPATH) $(DEP_INCPATH) $(CPPFLAGS) $(CXXFLAGS)  -o src/binlog_listener_err_packet.o src/err_packet.cpp

src/binlog_listener_handshake_init_packet.o:src/handshake_init_packet.cpp \
  include/handshake_init_packet.h \
  include/packet.h \
  include/network_exception.h \
  include/myexception.h \
  include/bytes_helper.h
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40msrc/binlog_listener_handshake_init_packet.o[0m']"
	$(CXX) -c $(INCPATH) $(DEP_INCPATH) $(CPPFLAGS) $(CXXFLAGS)  -o src/binlog_listener_handshake_init_packet.o src/handshake_init_packet.cpp

src/binlog_listener_handshake_response_packet.o:src/handshake_response_packet.cpp \
  include/handshake_response_packet.h \
  include/packet.h \
  include/network_exception.h \
  include/myexception.h \
  include/bytes_helper.h
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40msrc/binlog_listener_handshake_response_packet.o[0m']"
	$(CXX) -c $(INCPATH) $(DEP_INCPATH) $(CPPFLAGS) $(CXXFLAGS)  -o src/binlog_listener_handshake_response_packet.o src/handshake_response_packet.cpp

src/binlog_listener_mysql_replicator_connector.o:src/mysql_replicator_connector.cpp \
  include/mysql_replicator_connector.h \
  include/mysql_replicator_statement.h \
  include/mysql_replicator_resultset.h \
  include/packet_header.h \
  include/packet.h \
  include/network_exception.h \
  include/myexception.h \
  include/handshake_init_packet.h \
  include/handshake_response_packet.h \
  include/ok_packet.h \
  include/err_packet.h \
  include/eof_packet.h \
  include/network_exception.h \
  include/bytes_helper.h
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40msrc/binlog_listener_mysql_replicator_connector.o[0m']"
	$(CXX) -c $(INCPATH) $(DEP_INCPATH) $(CPPFLAGS) $(CXXFLAGS)  -o src/binlog_listener_mysql_replicator_connector.o src/mysql_replicator_connector.cpp

src/binlog_listener_mysql_replicator_resultset.o:src/mysql_replicator_resultset.cpp \
  include/mysql_replicator_resultset.h
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40msrc/binlog_listener_mysql_replicator_resultset.o[0m']"
	$(CXX) -c $(INCPATH) $(DEP_INCPATH) $(CPPFLAGS) $(CXXFLAGS)  -o src/binlog_listener_mysql_replicator_resultset.o src/mysql_replicator_resultset.cpp

src/binlog_listener_mysql_replicator_statement.o:src/mysql_replicator_statement.cpp \
  include/mysql_replicator_statement.h \
  include/mysql_replicator_resultset.h \
  include/bytes_helper.h \
  include/packet.h \
  include/network_exception.h \
  include/myexception.h \
  include/query_packet.h \
  include/mysql_replicator_com.h \
  include/ok_packet.h \
  include/err_packet.h \
  include/query_response_packet.h \
  include/packet_header.h
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40msrc/binlog_listener_mysql_replicator_statement.o[0m']"
	$(CXX) -c $(INCPATH) $(DEP_INCPATH) $(CPPFLAGS) $(CXXFLAGS)  -o src/binlog_listener_mysql_replicator_statement.o src/mysql_replicator_statement.cpp

src/binlog_listener_ok_packet.o:src/ok_packet.cpp \
  include/ok_packet.h \
  include/packet.h \
  include/network_exception.h \
  include/myexception.h \
  include/bytes_helper.h
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40msrc/binlog_listener_ok_packet.o[0m']"
	$(CXX) -c $(INCPATH) $(DEP_INCPATH) $(CPPFLAGS) $(CXXFLAGS)  -o src/binlog_listener_ok_packet.o src/ok_packet.cpp

src/binlog_listener_packet_header.o:src/packet_header.cpp \
  include/packet_header.h \
  include/packet.h \
  include/network_exception.h \
  include/myexception.h \
  include/bytes_helper.h
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40msrc/binlog_listener_packet_header.o[0m']"
	$(CXX) -c $(INCPATH) $(DEP_INCPATH) $(CPPFLAGS) $(CXXFLAGS)  -o src/binlog_listener_packet_header.o src/packet_header.cpp

src/binlog_listener_query_packet.o:src/query_packet.cpp \
  include/query_packet.h \
  include/packet.h \
  include/network_exception.h \
  include/myexception.h \
  include/mysql_replicator_com.h \
  include/bytes_helper.h
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40msrc/binlog_listener_query_packet.o[0m']"
	$(CXX) -c $(INCPATH) $(DEP_INCPATH) $(CPPFLAGS) $(CXXFLAGS)  -o src/binlog_listener_query_packet.o src/query_packet.cpp

src/binlog_listener_query_response_packet.o:src/query_response_packet.cpp \
  include/query_response_packet.h \
  include/packet.h \
  include/network_exception.h \
  include/myexception.h \
  include/bytes_helper.h
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40msrc/binlog_listener_query_response_packet.o[0m']"
	$(CXX) -c $(INCPATH) $(DEP_INCPATH) $(CPPFLAGS) $(CXXFLAGS)  -o src/binlog_listener_query_response_packet.o src/query_response_packet.cpp

endif #ifeq ($(shell uname -m),x86_64)

