package com.rusefi.output;

import com.rusefi.ReaderState;

import java.io.*;

/**
 * [OutputChannels]
 */
public class OutputsSectionConsumer implements ConfigurationConsumer {
    private final String tsOutputsSectionFileName;
    private final TsOutput tsOutput;
    private final ReaderState state;

    public OutputsSectionConsumer(String tsOutputsSectionFileName, ReaderState state) {
        this.tsOutputsSectionFileName = tsOutputsSectionFileName;
        tsOutput = new TsOutput(state, false);
        this.state = state;
    }

    public String getContent() {
        return tsOutput.getContent();
    }

    @Override
    public void handleEndStruct(ReaderState readerState, ConfigStructure structure) throws IOException {
        System.out.println("handleEndStruct");

        tsOutput.run(readerState, structure, 0);

        if (state.stack.isEmpty()) {
            if (tsOutputsSectionFileName != null) {
                FileWriter fos = new FileWriter(tsOutputsSectionFileName);
                fos.write(tsOutput.getContent());
                fos.close();
            }
        }
    }
}
