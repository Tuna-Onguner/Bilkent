`timescale 1ns / 1ps

//
// Bilkent University
// Fall 2022-2023
//
// CS 224: Computer Organization
// Lab 4
//
// Deniz Tuna Onguner
// Bilkent ID: 22001788
//


module mipslite_main (input  logic       CLK, RESET, next_instr,
                      output logic       memwrite, dp,
                      output logic [6:0] seg,
                      output logic [3:0] an);

    logic [31:0] pc, dataadr, writedata, readdata, instr;
    logic        clk, reset;
    
    top top (clk, reset, writedata, dataadr, 
             pc, instr, readdata, memwrite);
             
    pulse_controller pc_clk (CLK, next_instr, 0, clk);
    pulse_controller pc_reset (CLK, RESET, 0, reset);
             
    display_controller dc (CLK, pc[7:4], pc[3:0], dataadr[7:4], 
                           dataadr[3:0], seg, dp, an);
    
endmodule



module pulse_controller (input      CLK, sw_input, clear,
	                 output reg clk_pulse);

	 reg [ 2:0] state, nextstate;
	 reg [27:0] CNT; 
	 wire       cnt_zero; 

	always @(posedge CLK, posedge clear)
	   if(clear)
	    	state <=3'b000;
	   else
	    	state <= nextstate;

	always @ (sw_input, state, cnt_zero)
          case (state)
             3'b000: begin if (sw_input) nextstate = 3'b001; 
                           else nextstate = 3'b000; clk_pulse = 0; end	     
             3'b001: begin nextstate = 3'b010; clk_pulse = 1; end
             3'b010: begin if (cnt_zero) nextstate = 3'b011; 
                           else nextstate = 3'b010; clk_pulse = 1; end
             3'b011: begin if (sw_input) nextstate = 3'b011; 
                           else nextstate = 3'b100; clk_pulse = 0; end
             3'b100: begin if (cnt_zero) nextstate = 3'b000; 
                           else nextstate = 3'b100; clk_pulse = 0; end
            default: begin nextstate = 3'b000; clk_pulse = 0; end
          endcase

	always @(posedge CLK)
	   case(state)
		3'b001: CNT <= 100000000;
		3'b010: CNT <= CNT-1;
		3'b011: CNT <= 100000000;
		3'b100: CNT <= CNT-1;
	   endcase

	// reduction operator |CNT gives the OR of all bits in the CNT register	
	assign cnt_zero = ~|CNT;

endmodule



module display_controller (input  logic       clk,
                           input  logic [3:0] in3, in2, in1, in0,
                           output logic [6:0] seg, logic dp,
                           output logic [3:0] an);

    localparam N = 18;

    logic [N-1:0] count = {N{1'b0}};
    
    always @(posedge clk)
        count <= count + 1;

    logic [4:0] digit_val;
    logic [3:0] digit_en;
    
    always @(*) begin
        digit_en = 4'b1111;
        digit_val = in0;

        case(count[N-1:N-2])
            2'b00:	//select first 7Seg.
                begin
                    digit_val = {1'b0, in0};
                    digit_en = 4'b1110;
                end

            2'b01:	//select second 7Seg.
                begin
                    digit_val = {1'b0, in1};
                    digit_en = 4'b1101;
                end

            2'b10:	//select third 7Seg.
                begin
                    digit_val = {1'b0, in2};
                    digit_en = 4'b1011;
                end

            2'b11:	//select forth 7Seg.
                begin
                    digit_val = {1'b0, in3};
                    digit_en = 4'b0111;
                end
        endcase
    end

    // convert digit number to LED vector. LEDs are active low.

    logic [6:0] sseg_LEDs;
    
    always @(*) begin
        sseg_LEDs = 7'b1111111; //default
        case(digit_val)
            5'd0 : sseg_LEDs = 7'b1000000; //to display 0
            5'd1 : sseg_LEDs = 7'b1111001; //to display 1
            5'd2 : sseg_LEDs = 7'b0100100; //to display 2
            5'd3 : sseg_LEDs = 7'b0110000; //to display 3
            5'd4 : sseg_LEDs = 7'b0011001; //to display 4
            5'd5 : sseg_LEDs = 7'b0010010; //to display 5
            5'd6 : sseg_LEDs = 7'b0000010; //to display 6
            5'd7 : sseg_LEDs = 7'b1111000; //to display 7
            5'd8 : sseg_LEDs = 7'b0000000; //to display 8
            5'd9 : sseg_LEDs = 7'b0010000; //to display 9
            5'd10: sseg_LEDs = 7'b0001000; //to display a
            5'd11: sseg_LEDs = 7'b0000011; //to display b
            5'd12: sseg_LEDs = 7'b1000110; //to display c
            5'd13: sseg_LEDs = 7'b0100001; //to display d
            5'd14: sseg_LEDs = 7'b0000110; //to display e
            5'd15: sseg_LEDs = 7'b0001110; //to display f
            5'd16: sseg_LEDs = 7'b0110111; //to display "="
          default: sseg_LEDs = 7'b0111111; //dash 
        endcase
    end

    assign an = digit_en;
    assign seg = sseg_LEDs;
    assign dp = 1'b1; // turn dp off

endmodule



module top (input  logic        clk, reset,      
	        output logic [31:0] writedata, dataadr,
	        output logic [31:0] pc, instr, readdata,
	        output logic        memwrite);

   // instantiate processor and memories  
   mips mips (clk, reset, pc, instr, memwrite, 
              dataadr, writedata, readdata);  
   imem imem (pc[7:0], instr);  
   dmem dmem (clk, memwrite, dataadr, 
              writedata, readdata);

endmodule



// External data memory used by MIPS single-cycle processor
module dmem (input  logic        clk, we,
             input  logic [31:0] a, wd,
             output logic [31:0] rd);

   logic [31:0] RAM [63:0];
  
   assign rd = RAM[a[31:2]];    // word-aligned  read (for lw)

   always_ff @(posedge clk)
     if (we)
       RAM[a[31:2]] <= wd;      // word-aligned write (for sw)

endmodule



// External instruction memory used by MIPS single-cycle
// processor. It models instruction memory as a stored-program 
// ROM, with address as input, and instruction as output
module imem (input  logic [ 7:0] addr,
             output logic [31:0] instr);

// imem is modeled as a lookup table, a stored-program byte-addressable ROM
	always_comb
	   case (addr)		   	// word-aligned fetch
//		address		    instruction
//		-------		    -----------
		8'h00: instr = 32'h20020005;  	// disassemble, by hand 
		8'h04: instr = 32'h2003000c;  	// or with a program,
		8'h08: instr = 32'h1843000c;
		8'h0c: instr = 32'h2067fff7;    // to find out what
		8'h10: instr = 32'h00e22025;  	// this program does!
		8'h14: instr = 32'h00642824;
		8'h18: instr = 32'h00a42820;
		//8'h18: instr = 32'h10a7000a;
		8'h1c: instr = 32'h0064202a;
		//8'h20: instr = 32'h10800001;
		8'h20: instr = 32'h20050000;
		8'h24: instr = 32'h00e2202a;
		8'h28: instr = 32'h00853820;
		8'h2c: instr = 32'h00e23822;
		8'h30: instr = 32'hac670044;
		8'h34: instr = 32'h8c020050;
		8'h38: instr = 32'h08000011;
		8'h3c: instr = 32'h20020001;
		8'h40: instr = 32'hac020054;
		8'h44: instr = 32'h2009001c;
		8'h48: instr = 32'h0d200008;
	    default:  instr = {32{1'bx}};	// unknown address
	   endcase

endmodule



// single-cycle MIPS processor, with controller and datapath
module mips (input  logic         clk, reset,
             output logic [31:0]  pc,
             input  logic [31:0]  instr,
             output logic         memwrite,
             output logic [31:0]  aluout, writedata,
             input  logic [31:0]  readdata);

  logic       memtoreg, pcsrc, zero, alusrc, regdst, regwrite, jump, jm;
  logic [2:0] alucontrol;

  controller c (instr[31:26], instr[5:0], zero, memtoreg, memwrite, 
                pcsrc, alusrc, regdst, regwrite, jump, jm, alucontrol);

  datapath dp (clk, reset, memtoreg, pcsrc, alusrc, regdst, regwrite, jump, jm,
               alucontrol, zero, pc, instr, aluout, writedata, readdata);

endmodule



module controller (input  logic [5:0] op, funct,
                   input  logic       zero,
                   output logic       memtoreg, memwrite,
                   output logic       pcsrc, alusrc,
                   output logic       regdst, regwrite,
                   output logic       jump, jm,
                   output logic [2:0] alucontrol);

   logic [1:0] aluop;
   logic       branch;

   maindec md (op, memtoreg, memwrite, branch, alusrc, 
               regdst, regwrite, jump, jm, aluop);

   aludec ad (funct, aluop, alucontrol);

   assign pcsrc = branch & zero;

endmodule



module maindec (input  logic [5:0] op, 
	            output logic       memtoreg, memwrite, branch,
	            output logic       alusrc, regdst, regwrite, jump, jm,
	            output logic [1:0] aluop);
	            
   logic [9:0] controls;

   assign {regwrite, regdst, alusrc, branch, 
           memwrite, memtoreg, aluop, jump, jm} = controls;

  always_comb
    case(op)
      6'b000000: controls <= 10'b1100001000; // R-type
      6'b100011: controls <= 10'b1010010000; // LW
      6'b101011: controls <= 10'b0010100000; // SW
      6'b000110: controls <= 10'b0001001100; // BLT
      6'b000100: controls <= 10'b0001000100; // BEQ
      6'b001000: controls <= 10'b1010000000; // ADDI
      6'b000010: controls <= 10'b0000000010; // J
      6'b000011: controls <= 10'b0010000001; // JM
      default:   controls <= 10'bxxxxxxxxxx; // illegal op
    endcase
endmodule



module aludec (input  logic [5:0] funct,
               input  logic [1:0] aluop,
               output logic [2:0] alucontrol);
               
  always_comb
    case(aluop)
      2'b00: alucontrol = 3'b010;  // add  (for lw/sw/addi)
      2'b01: alucontrol = 3'b110;  // sub  (for beq)
      2'b11: alucontrol = 3'b101;  // slt  (for blt)
      default: case(funct)         // R-TYPE instructions
          6'b100000: alucontrol = 3'b010; // ADD
          6'b100010: alucontrol = 3'b110; // SUB
          6'b100100: alucontrol = 3'b000; // AND
          6'b100101: alucontrol = 3'b001; // OR
          6'b101010: alucontrol = 3'b111; // SLT
          default:   alucontrol = 3'bxxx; // ???
        endcase
    endcase
    
endmodule



module datapath (input  logic        clk, reset, memtoreg, pcsrc, alusrc, regdst,
                 input  logic        regwrite, jump, jm,
		 input  logic [ 2:0] alucontrol,
                 output logic        zero,
		 output logic [31:0] pc,
	         input  logic [31:0] instr,
                 output logic [31:0] aluout, writedata,
	         input  logic [31:0] readdata);

  logic [ 4:0] writereg;
  logic [31:0] pcnext, pcmain, pcnextbr, pcplus4, pcbranch;
  logic [31:0] signimm, signimmsh, srca, srcb, result;
 
  // next PC logic
  flopr #(32)  pcreg (clk, reset, pcnext, pc);
  adder        pcadd1 (pc, 32'b100, pcplus4);
  sl2          immsh (signimm, signimmsh);
  adder        pcadd2 (pcplus4, signimmsh, pcbranch);
  mux2 #(32)   pcbrmux (pcplus4, pcbranch, pcsrc,
                        pcnextbr);
  mux2 #(32)   pcmux(pcnextbr, {pcplus4[31:28], 
                     instr[25:0], 2'b00}, jump, pcmain);
  mux2 #(32)   mainmux (pcmain, result, jm, pcnext);

   // register file logic
   regfile     rf (clk, regwrite, instr[25:21], instr[20:16], writereg,
                   result, srca, writedata);

   mux2 #(5)   wrmux (instr[20:16], instr[15:11], regdst, writereg);
   mux2 #(32)  resmux (aluout, readdata, memtoreg, result);
   signext     se (instr[15:0], signimm);

   // ALU logic
   mux2 #(32)  srcbmux (writedata, signimm, alusrc, srcb);
   alu         alu (srca, srcb, alucontrol, aluout, zero);

endmodule



module regfile (input  logic        clk, we3, 
                input  logic [ 4:0] ra1, ra2, wa3, 
                input  logic [31:0] wd3, 
                output logic [31:0] rd1, rd2);

  logic [31:0] rf [31:0];

  // three ported register file: read two ports combinationally
  // write third port on rising edge of clock. Register0 hardwired to 0.

  always_ff@(posedge clk)
     if (we3) 
         rf [wa3] <= wd3;	

  assign rd1 = (ra1 != 0) ? rf [ra1] : 0;
  assign rd2 = (ra2 != 0) ? rf[ ra2] : 0;

endmodule



module alu (input  logic [31:0] a, b, 
            input  logic [ 2:0] alucont, 
            output logic [31:0] result,
            output logic zero);
    
    always_comb
        case(alucont)
            3'b010: result = a + b;
            3'b110: result = a - b;
            3'b000: result = a & b;
            3'b001: result = a | b;
            3'b101: result = (a  > b) ? 1 : 0;
            3'b111: result = (a  < b) ? 1 : 0;
            3'b011: result = (a == b) ? 0 : 1;
            3'b100: result = a << b;
            default: result = {32{1'bx}};
        endcase
    
    assign zero = (result == 0) ? 1'b1 : 1'b0;
endmodule



module adder (input  logic [31:0] a, b,
              output logic [31:0] y);
     
     assign y = a + b;

endmodule



module sl2 (input  logic [31:0] a,
            output logic [31:0] y);
     
     assign y = {a[29:0], 2'b00}; // shifts left by 2
     
endmodule



module signext (input  logic [15:0] a,
                output logic [31:0] y);
              
  assign y = {{16{a[15]}}, a};    // sign-extends 16-bit a
  
endmodule



// parameterized register
module flopr #(parameter WIDTH = 8)
              (input  logic             clk, reset, 
	       input  logic [WIDTH-1:0] d, 
               output logic [WIDTH-1:0] q);

  always_ff @(posedge clk, posedge reset)
    if (reset) q <= 0; 
    else       q <= d;
    
endmodule



// paramaterized 2-to-1 MUX
module mux2 #(parameter WIDTH = 8)
             (input  logic [WIDTH-1:0] d0, d1,  
              input  logic             s, 
              output logic [WIDTH-1:0] y);
  
   assign y = s ? d1 : d0;
   
endmodule