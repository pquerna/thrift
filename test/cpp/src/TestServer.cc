#include <concurrency/ThreadManager.h>
#include <concurrency/PosixThreadFactory.h>
#include <protocol/TBinaryProtocol.h>
#include <server/TSimpleServer.h>
#include <server/TThreadPoolServer.h>
#include <transport/TServerSocket.h>
#include "ThriftTest.h"

#include <iostream>
#include <stdexcept>
#include <sstream>

using namespace std;

using namespace facebook::thrift;
using namespace facebook::thrift::protocol;
using namespace facebook::thrift::transport;
using namespace facebook::thrift::server;

using namespace thrift::test;

class TestHandler : public ThriftTestIf {
 public:
  TestHandler() {}

  void testVoid() {
    printf("testVoid()\n");
  }

  string testString(string thing) {
    printf("testString(\"%s\")\n", thing.c_str());
    return thing;
  }

  int8_t testByte(int8_t thing) {
    printf("testByte(%d)\n", (int)thing);
    return thing;
  }

  int32_t testI32(int32_t thing) {
    printf("testI32(%d)\n", thing);
    return thing;
  }

  int64_t testI64(int64_t thing) {
    printf("testI64(%ld)\n", thing);
    return thing;
  }

  double testDouble(double thing) {
    printf("testDouble(%lf)\n", thing);
    return thing;
  }

  Xtruct testStruct(Xtruct thing) {
    printf("testStruct({\"%s\", %d, %d, %ld})\n",
           thing.string_thing.c_str(),
           (int)thing.byte_thing,
           thing.i32_thing,
           thing.i64_thing);
    return thing;
  }

  Xtruct2 testNest(Xtruct2 nest) {
    Xtruct thing = nest.struct_thing;
    printf("testNest({%d, {\"%s\", %d, %d, %ld}, %d})\n",
           (int)nest.byte_thing,
           thing.string_thing.c_str(),
           (int)thing.byte_thing,
           thing.i32_thing,
           thing.i64_thing,
           nest.i32_thing);
    return nest;
  }

  map<int32_t, int32_t> testMap(map<int32_t, int32_t> thing) {
    printf("testMap({");
    map<int32_t, int32_t>::const_iterator m_iter;
    bool first = true;
    for (m_iter = thing.begin(); m_iter != thing.end(); ++m_iter) {
      if (first) {
        first = false;
      } else {
        printf(", ");
      }
      printf("%d => %d", m_iter->first, m_iter->second);
    }
    printf("})\n");
    return thing;
  }

  set<int32_t> testSet(set<int32_t> thing) {
    printf("testSet({");
    set<int32_t>::const_iterator s_iter;
    bool first = true;
    for (s_iter = thing.begin(); s_iter != thing.end(); ++s_iter) {
      if (first) {
        first = false;
      } else {
        printf(", ");
      }
      printf("%d", *s_iter);
    }
    printf("})\n");
    return thing;
  }

  list<int32_t> testList(list<int32_t> thing) {
    printf("testList({");
    list<int32_t>::const_iterator l_iter;
    bool first = true;
    for (l_iter = thing.begin(); l_iter != thing.end(); ++l_iter) {
      if (first) {
        first = false;
      } else {
        printf(", ");
      }
      printf("%d", *l_iter);
    }
    printf("})\n");
    return thing;
  }

  Numberz testEnum(Numberz thing) {
    printf("testEnum(%d)\n", thing);
    return thing;
  }

  UserId testTypedef(UserId thing) {
    printf("testTypedef(%ld)\n", thing);
    return thing;
  }

  map<int32_t, map<int32_t,int32_t> > testMapMap(int32_t hello) {
    printf("testMapMap(%d)\n", hello);
    map<int32_t, map<int32_t,int32_t> > mapmap;

    map<int32_t,int32_t> pos;
    map<int32_t,int32_t> neg;
    for (int i = 1; i < 5; i++) {
      pos.insert(make_pair(i,i));
      neg.insert(make_pair(-i,-i));
    }

    mapmap.insert(make_pair(4, pos));
    mapmap.insert(make_pair(-4, neg));

    return mapmap;
  }

  map<UserId, map<Numberz,Insanity> > testInsanity(Insanity argument) {
    printf("testInsanity()\n");
    
    Xtruct hello;
    hello.string_thing = "Hello2";
    hello.byte_thing = 2;
    hello.i32_thing = 2;
    hello.i64_thing = 2;

    Xtruct goodbye;
    goodbye.string_thing = "Goodbye4";
    goodbye.byte_thing = 4;
    goodbye.i32_thing = 4;
    goodbye.i64_thing = 4;

    Insanity crazy;
    crazy.userMap.insert(make_pair(EIGHT, 8));
    crazy.xtructs.push_back(goodbye);

    Insanity looney;
    crazy.userMap.insert(make_pair(FIVE, 5));
    crazy.xtructs.push_back(hello);

    map<Numberz, Insanity> first_map;
    map<Numberz, Insanity> second_map;

    first_map.insert(make_pair(TWO, crazy));
    first_map.insert(make_pair(THREE, crazy));

    second_map.insert(make_pair(SIX, looney));

    map<UserId, map<Numberz,Insanity> > insane;
    insane.insert(make_pair(1, first_map));
    insane.insert(make_pair(2, second_map));

    printf("return");
    printf(" = {");
    map<UserId, map<Numberz,Insanity> >::const_iterator i_iter;
    for (i_iter = insane.begin(); i_iter != insane.end(); ++i_iter) {
      printf("%ld => {", i_iter->first);
      map<Numberz,Insanity>::const_iterator i2_iter;
      for (i2_iter = i_iter->second.begin();
           i2_iter != i_iter->second.end();
           ++i2_iter) {
        printf("%d => {", i2_iter->first);
        map<Numberz, UserId> userMap = i2_iter->second.userMap;
        map<Numberz, UserId>::const_iterator um;
        printf("{");
        for (um = userMap.begin(); um != userMap.end(); ++um) {
          printf("%d => %ld, ", um->first, um->second);
        }
        printf("}, ");

        list<Xtruct> xtructs = i2_iter->second.xtructs;
        list<Xtruct>::const_iterator x;
        printf("{");
        for (x = xtructs.begin(); x != xtructs.end(); ++x) {
          printf("{\"%s\", %d, %d, %ld}, ",
                 x->string_thing.c_str(),
                 (int)x->byte_thing,
                 x->i32_thing,
                 x->i64_thing);
        }
        printf("}");

        printf("}, ");
      }
      printf("}, ");
    }
    printf("}\n");

    return insane;
  }

  Xtruct testMulti(int8_t arg0, int32_t arg1, int64_t arg2, std::map<int16_t, std::string>  arg3, Numberz arg4, UserId arg5) {
    printf("testMulti()\n");
    
    Xtruct hello;
    hello.string_thing = "Hello2";
    hello.byte_thing = arg0;
    hello.i32_thing = arg1;
    hello.i64_thing = (int64_t)arg2;

    return hello;
  }

  void testException(std::string arg) throw(Xception) {
    printf("testException(%s)\n", arg.c_str());
    if (arg.compare("Xception") == 0) {
      Xception e;
      e.errorCode = 1001;
      e.message = "This is an Xception";
      throw e;
    } else {
      Xtruct result;
      result.string_thing = arg;
      return;
    }
  }
  
  Xtruct testMultiException(std::string arg0, std::string arg1) throw(Xception, Xception2) {

    printf("testMultiException(%s, %s)\n", arg0.c_str(), arg1.c_str());

    if (arg0.compare("Xception") == 0) {
      Xception e;
      e.errorCode = 1001;
      e.message = "This is an Xception";
      throw e;
    } else if (arg0.compare("Xception2") == 0) {
      Xception2 e;
      e.errorCode = 2002;
      e.struct_thing.string_thing = "This is an Xception2";
      throw e;
    } else {
      Xtruct result;
      result.string_thing = arg1;
      return  result;
    }
  }
};

int main(int argc, char **argv) {

  int port = 9090;
  string serverType = "simple";
  string protocolType = "binary";
  size_t workerCount = 4;

  ostringstream usage;

  usage <<
    argv[0] << " [--port=<port number>] [--server-type=<server-type>] [--protocol-type=<protocol-type>] [--workers=<worker-count>]" << endl <<

    "\t\tserver-type\t\ttype of server, \"simple\" or \"thread-pool\".  Default is " << serverType << endl <<

    "\t\tprotocol-type\t\ttype of protocol, \"binary\", \"ascii\", or \"xml\".  Default is " << protocolType << endl <<

    "\t\tworkers\t\tNumber of thread pools workers.  Only valid for thread-pool server type.  Default is " << workerCount << endl;
    
  map<string, string>  args;
  
  for (int ix = 1; ix < argc; ix++) {
    string arg(argv[ix]);
    if (arg.compare(0,2, "--") == 0) {
      size_t end = arg.find_first_of("=", 2);
      if (end != string::npos) {
	args[string(arg, 2, end - 2)] = string(arg, end + 1);
      } else {
	args[string(arg, 2, end - 2)] = "true";
      }
      ix++;
    } else {
      throw invalid_argument("Unexcepted command line token: "+arg);
    }
  }

  try {

    if (!args["port"].empty()) {
      port = atoi(args["port"].c_str());
    }

    if (!args["server-type"].empty()) {
      serverType = args["server-type"];     
      if (serverType == "simple") {
      } else if (serverType == "thread-pool") {
      } else {
	throw invalid_argument("Unknown server type "+serverType);
      }
    }

    if (!args["protocol-type"].empty()) {
      protocolType = args["protocol-type"];
      if (protocolType == "binary") {
      } else if (protocolType == "ascii") {
	throw invalid_argument("ASCII protocol not supported");
      } else if (protocolType == "xml") {
	throw invalid_argument("XML protocol not supported");
      } else {
	throw invalid_argument("Unknown protocol type "+protocolType);
      }
    } 

    if (!args["workers"].empty()) {
      workerCount = atoi(args["workers"].c_str());
    }
  } catch (exception& e) {
    cerr << e.what() << endl;
    cerr << usage;
  }

  // Dispatcher
  shared_ptr<TBinaryProtocol> binaryProtocol(new TBinaryProtocol);

  shared_ptr<TestHandler> testHandler(new TestHandler());

  shared_ptr<ThriftTestServer> testServer(new ThriftTestServer(testHandler, binaryProtocol));

  // Options
  shared_ptr<TServerOptions> serverOptions(new TServerOptions());

  // Transport
  shared_ptr<TServerSocket> serverSocket(new TServerSocket(port));

  if (serverType == "simple") {

    // Server
    TSimpleServer simpleServer(testServer,
			       serverOptions,
			       serverSocket);

    printf("Starting the server on port %d...\n", port);
    simpleServer.run();

  } else if (serverType == "thread-pool") {

    shared_ptr<ThreadManager> threadManager =
      ThreadManager::newSimpleThreadManager(workerCount);

    shared_ptr<PosixThreadFactory> threadFactory =
      shared_ptr<PosixThreadFactory>(new PosixThreadFactory());

    threadManager->threadFactory(threadFactory);

    threadManager->start();

    TThreadPoolServer threadPoolServer(testServer,
				       serverOptions,
				       serverSocket,
				       threadManager);

    printf("Starting the server on port %d...\n", port);
    threadPoolServer.run();
  }

  printf("done.\n");
  return 0;
}