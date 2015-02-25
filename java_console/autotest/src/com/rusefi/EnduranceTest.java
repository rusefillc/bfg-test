package com.rusefi;

import static com.rusefi.IoUtil.sendCommand;
import static com.rusefi.IoUtil.sleep;
import static com.rusefi.RealHwTest.startRealHardwareTest;

public class EnduranceTest {

    private static final int COUNT = 200;

    public static void main(String[] args) {
        long start = System.currentTimeMillis();
        try {
            String port = startRealHardwareTest(args);

            if (port == null)
                return;

            IoUtil.realHardwareConnect(port);
            for (int i = 0; i < COUNT; i++) {
                AutoTest.currentEngineType = 3;
                sendCommand("set_engine_type " + 3, AutoTest.COMPLEX_COMMAND_RETRY, 600);
                sleep(2);
                sendCommand("enable self_stimulation");
//                IoUtil.changeRpm(1200);
                AutoTest.currentEngineType = 28;
                sendCommand("set_engine_type " + 28, AutoTest.COMPLEX_COMMAND_RETRY, 600);
                sleep(2);
                FileLog.MAIN.logLine("++++++++++++++++++++++++++++++++++++  " + i + "   +++++++++++++++");
            }

        } catch (Throwable e) {
            e.printStackTrace();
            System.exit(-1);
        }
        FileLog.MAIN.logLine("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
        FileLog.MAIN.logLine("++++++++++++++++++++++++++++++++++++  YES YES YES " + COUNT + "   +++++++++++++++");
        FileLog.MAIN.logLine("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
        FileLog.MAIN.logLine("In " + (System.currentTimeMillis() - start) + "ms");
    }
}