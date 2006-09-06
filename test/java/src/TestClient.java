package com.facebook.thrift.test;

// Generated code
import thrift.test.*;

import com.facebook.thrift.transport.TSocket;
import com.facebook.thrift.transport.TTransportException;
import com.facebook.thrift.protocol.TBinaryProtocol;

import java.util.HashMap;
import java.util.HashSet;
import java.util.ArrayList;

/**
 * Test Java client for thrift. Essentially just a copy of the C++ version,
 * this makes a variety of requests to enable testing for both performance and
 * correctness of the output.
 *
 * @author Mark Slee <mcslee@facebook.com>
 */
public class TestClient {
  public static void main(String [] args) {
    try {
      String host = "localhost";
      int port = 9090;
      int numTests = 1;
      
      if (args.length > 0) {
        host = args[0];
      }
      if (args.length > 1) {
        port = Integer.valueOf(args[1]);
      }
      if (args.length > 2) {
        numTests = Integer.valueOf(args[2]);
      }
      
      TSocket tSocket =
        new TSocket(host, port);
      TBinaryProtocol binaryProtocol =
        new TBinaryProtocol();
      ThriftTest.Client testClient =
        new ThriftTest.Client(tSocket, binaryProtocol);

      for (int test = 0; test < numTests; ++test) {

        /**
         * CONNECT TEST
         */
        System.out.println("Test #" + (test+1) + ", " +
                           "connect " + host + ":" + port);
        try {
          tSocket.open();
        } catch (TTransportException ttx) {
          System.out.println("Connect failed: " + ttx.getMessage());
          continue;
        }

        long start = System.currentTimeMillis();
    
        /**
         * VOID TEST
         */
        System.out.print("testVoid()");
        testClient.testVoid();
        System.out.print(" = void\n");

        /**
         * STRING TEST
         */
        System.out.print("testString(\"Test\")");
        String s = testClient.testString("Test");
        System.out.print(" = \"" + s + "\"\n");

        /**
         * BYTE TEST
         */
        System.out.print("testByte(1)");
        byte i8 = testClient.testByte((byte)1);
        System.out.print(" = " + i8 + "\n");
  
        /**
         * I32 TEST
         */
        System.out.print("testI32(-1)");
        int i32 = testClient.testI32(-1);
        System.out.print(" = " + i32 + "\n");

        /**
         * I64 TEST
         */
        System.out.print("testI64(-34359738368)");
        long i64 = testClient.testI64(-34359738368L);
        System.out.print(" = " + i64 + "\n");

        /**
         * DOUBLE TEST
         */
        System.out.print("testDouble(5.325098235)");
        double dub = testClient.testDouble(5.325098235);
        System.out.print(" = " + dub + "\n");

        /**
         * STRUCT TEST
         */
        System.out.print("testStruct({\"Zero\", 1, -3, -5})");
        Xtruct out = new Xtruct();
        out.string_thing = "Zero";
        out.byte_thing = (byte) 1;
        out.i32_thing = -3;
        out.i64_thing = -5;
        Xtruct in = testClient.testStruct(out);
        System.out.print(" = {" +
                         "\"" + in.string_thing + "\", " +
                         in.byte_thing + ", " +
                         in.i32_thing + ", " +
                         in.i64_thing + "}\n");

        /**
         * NESTED STRUCT TEST
         */
        System.out.print("testNest({1, {\"Zero\", 1, -3, -5}), 5}");
        Xtruct2 out2 = new Xtruct2();
        out2.byte_thing = (short)1;
        out2.struct_thing = out;
        out2.i32_thing = 5;
        Xtruct2 in2 = testClient.testNest(out2);
        in = in2.struct_thing;
        System.out.print(" = {" +
                         in2.byte_thing + ", {" +
                         "\"" + in.string_thing + "\", " +
                         in.byte_thing + ", " +
                         in.i32_thing + ", " +
                         in.i64_thing + "}, " +
                         in2.i32_thing + "}\n");

        /**
         * MAP TEST
         */
        HashMap<Integer,Integer> mapout = new HashMap<Integer,Integer>();
        for (int i = 0; i < 5; ++i) {
          mapout.put(i, i-10);
        }
        System.out.print("testMap({");
        boolean first = true;
        for (int key : mapout.keySet()) {
          if (first) {
            first = false;
          } else {
            System.out.print(", ");
          }
          System.out.print(key + " => " + mapout.get(key));
        }
        System.out.print("})");
        HashMap<Integer,Integer> mapin = testClient.testMap(mapout);
        System.out.print(" = {");
        first = true;
        for (int key : mapin.keySet()) {
          if (first) {
            first = false;
          } else {
            System.out.print(", ");
          }
          System.out.print(key + " => " + mapout.get(key));
        }
        System.out.print("}\n");

        /**
         * SET TEST
         */
        HashSet<Integer> setout = new HashSet<Integer>();
        for (int i = -2; i < 3; ++i) {
          setout.add(i);
        }
        System.out.print("testSet({");
        first = true;
        for (int elem : setout) {
          if (first) {
            first = false;
          } else {
            System.out.print(", ");
          }
          System.out.print(elem);
        }
        System.out.print("})");
        HashSet<Integer> setin = testClient.testSet(setout);
        System.out.print(" = {");
        first = true;
        for (int elem : setin) {
          if (first) {
            first = false;
          } else {
            System.out.print(", ");
          }
          System.out.print(elem);
        }
        System.out.print("}\n");

        /**
         * LIST TEST
         */
        ArrayList<Integer> listout = new ArrayList<Integer>();
        for (int i = -2; i < 3; ++i) {
          listout.add(i);
        }
        System.out.print("testList({");
        first = true;
        for (int elem : listout) {
          if (first) {
            first = false;
          } else {
            System.out.print(", ");
          }
          System.out.print(elem);
        }
        System.out.print("})");
        ArrayList<Integer> listin = testClient.testList(listout);
        System.out.print(" = {");
        first = true;
        for (int elem : listin) {
          if (first) {
            first = false;
          } else {
            System.out.print(", ");
          }
          System.out.print(elem);
        }
        System.out.print("}\n");

        /**
         * ENUM TEST
         */
        System.out.print("testEnum(ONE)");
        int ret = testClient.testEnum(Numberz.ONE);
        System.out.print(" = " + ret + "\n");

        System.out.print("testEnum(TWO)");
        ret = testClient.testEnum(Numberz.TWO);
        System.out.print(" = " + ret + "\n");

        System.out.print("testEnum(THREE)");
        ret = testClient.testEnum(Numberz.THREE);
        System.out.print(" = " + ret + "\n");

        System.out.print("testEnum(FIVE)");
        ret = testClient.testEnum(Numberz.FIVE);
        System.out.print(" = " + ret + "\n");

        System.out.print("testEnum(EIGHT)");
        ret = testClient.testEnum(Numberz.EIGHT);
        System.out.print(" = " + ret + "\n");

        /**
         * TYPEDEF TEST
         */
        System.out.print("testTypedef(309858235082523)");
        long uid = testClient.testTypedef(309858235082523L);
        System.out.print(" = " + uid + "\n");

        /**
         * NESTED MAP TEST
         */
        System.out.print("testMapMap(1)");
        HashMap<Integer,HashMap<Integer,Integer>> mm =
          testClient.testMapMap(1);
        System.out.print(" = {");
        for (int key : mm.keySet()) {
          System.out.print(key + " => {");
          HashMap<Integer,Integer> m2 = mm.get(key);
          for (int k2 : m2.keySet()) {
            System.out.print(k2 + " => " + m2.get(k2) + ", ");
          }
          System.out.print("}, ");
        }
        System.out.print("}\n");

        /**
         * INSANITY TEST
         */
        Insanity insane = new Insanity();
        insane.userMap.put(Numberz.FIVE, (long)5000);
        Xtruct truck = new Xtruct();
        truck.string_thing = "Truck";
        truck.byte_thing = (byte)8;
        truck.i32_thing = 8;
        truck.i64_thing = 8;
        insane.xtructs.add(truck);
        System.out.print("testInsanity()");
        HashMap<Long,HashMap<Integer,Insanity>> whoa =
          testClient.testInsanity(insane);
        System.out.print(" = {");
        for (long key : whoa.keySet()) {
          HashMap<Integer,Insanity> val = whoa.get(key);
          System.out.print(key + " => {");

          for (int k2 : val.keySet()) {
            Insanity v2 = val.get(k2);
            System.out.print(k2 + " => {");
            HashMap<Integer, Long> userMap = v2.userMap;
            System.out.print("{");
            for (int k3 : userMap.keySet()) {
              System.out.print(k3 + " => " +
                               userMap.get(k3) + ", ");
            }
            System.out.print("}, ");

            ArrayList<Xtruct> xtructs = v2.xtructs;
            System.out.print("{");
            for (Xtruct x : xtructs) {
              System.out.print("{" +
                               "\"" + x.string_thing + "\", " +
                               x.byte_thing + ", " +
                               x.i32_thing + ", "+
                               x.i64_thing + "}, ");
            }
            System.out.print("}");

            System.out.print("}, ");
          }
          System.out.print("}, ");
        }
        System.out.print("}\n");

        long stop = System.currentTimeMillis();
        System.out.println("Total time: " + (stop-start) + "ms");

        tSocket.close();
      }
      
    } catch (Exception x) {
      x.printStackTrace();
    }
  }
}