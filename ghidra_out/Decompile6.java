// Ghidra post-script: decompile fixed addresses and dump C to /home/daiaji/repo/PECMD反编译/ghidra_out/
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionManager;
import java.io.FileWriter;
import java.io.File;

public class Decompile6 extends GhidraScript {
	public void run() throws Exception {
		DecompInterface di = new DecompInterface();
		di.openProgram(currentProgram);
		String[] addrs = {"0x140005c18","0x140034788","0x14005fc90","0x1400682b0","0x1400e4864","0x1400fdad8"};
		File outDir = new File("/home/daiaji/repo/PECMD反编译/ghidra_out");
		FunctionManager fm = currentProgram.getFunctionManager();
		for (String s : addrs) {
			try {
				Address a = currentProgram.getAddressFactory().getAddress(s);
				Function f = fm.getFunctionAt(a);
				FileWriter w = new FileWriter(new File(outDir, s.substring(2) + ".c"));
				if (f == null) { w.write("/* NO FUNCTION AT " + s + " */\n"); w.close(); continue; }
				DecompileResults r = di.decompileFunction(f, 60, monitor);
				if (r.decompileCompleted()) w.write(r.getDecompiledFunction().getC());
				else w.write("/* DECOMPILE INCOMPLETE */\n" + r.getErrorMessage());
				w.close();
				println("Done " + s);
			} catch (Exception e) { println("ERR " + s + " " + e.getMessage()); }
		}
	}
}
