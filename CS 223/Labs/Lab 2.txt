//
// Bilkent University
// 2021-2022, Spring 
//
// CS 223
// Lab 2
//
// Deniz Tuna Onguner
// ID: 22001788
//


module half_adder(input logic a, b, 
                 output logic s, c);
    assign s = a ^ b;
    assign c = a & b;
endmodule



module half_subtractor(input logic a, b,
                      output logic diff, brrw);
    assign diff =  a ^ b;
    assign brrw = ~a & b;
endmodule



module OR2input(input logic a, b, output logic y);
    assign y = a | b;
endmodule



module full_adder(input logic a, b, cin, 
                 output logic s, c);
    logic n0, n1, n2;
    half_adder ha1(a, b, n0, n1);
    half_adder ha2(n0, cin, s, n2);
    OR2input or2in(n2, n1, c);
endmodule



module two_bit_adder(input logic a0, a1, b0, b1, cin, 
                    output logic s0, s1, c);
    logic c0;
    full_adder fa1(a0, b0, cin, s0, c0);
    full_adder fa2(a1, b1, c0, s1, c);
endmodule



module XOR2input(input logic a, b, output logic y);
    assign y = a ^ b;
endmodule



module NAND2input(input logic a, b, output logic y);
    assign y = ~(a & b);
endmodule



module LabCalculator(input logic c, d, a, b, 
                    output logic y, z);
    logic sum, diff, cout, brrw, notin, xorin;
    half_adder add(a, b, sum, cout);
    half_subtractor sub(a, b, diff, brrw);
    XOR2input xin(a, b, xorin);
    NAND2input nan(a, b, notin);
    assign y = c ? ( d ? diff : sum ) : ( d ? notin : xorin);
    assign z = c ? ( d ? brrw : cout ) : 0;
endmodule



module AND3input(input logic a, b, c, output logic y);
    assign y = a & b & c;
endmodule



module LabCalc_Sol2(input logic c, d, a, b, 
                    output logic y, z);
    logic l1, l2, l3;
    XOR2input xn2(a, b, l1);
    AND3input an3(~b, ~c, d, l2);
    OR2input  or2(l1, l2, y);
    XOR2input xr2(a, d, l3);
    AND3input ad3(b, c, l3, z);
endmodule



module half_adder_TB();
    logic a, b;
    logic c, s;
    half_adder dut(a, b, s, c);
    initial begin
        a = 0; b = 0; #10;
        if (c != 0 | s != 0) $display("00 failed");
        b = 1; #10;
        if (c != 0 | s != 1) $display("01 failed");
        a = 1; #10;
        if (c != 1 | s != 0) $display("10 failed");
        b = 0; #10;
        if (c != 0 | s != 1) $display("11 failed");
    end
endmodule



module half_subtractor_TB();
    logic a, b, diff, brrw;
    half_subtractor dut(a, b, diff, brrw);
    initial begin
        a = 0; b = 0; #10;
        if (diff != 0 | brrw != 0) $display("00 failed");
        b = 1; #10;
        if (diff != 1 | brrw != 1) $display("01 failed");
        a = 1; #10;
        if (diff != 0 | brrw != 0) $display("10 failed");
        b = 0; #10;
        if (diff != 1 | brrw != 0) $display("11 failed");
    end
endmodule



module full_adder_TB();
    logic a, b, cin; 
    logic s, c;
    full_adder dut(a, b, cin, s, c);
    initial begin
        a = 0; b = 0; cin = 0; #10;
                      cin = 1; #10;
               b = 1; cin = 0; #10;
                      cin = 1; #10;
        a = 1; b = 0; cin = 0; #10;
                      cin = 1; #10;
               b = 1; cin = 0; #10;
                      cin = 1; #10;     
    end
endmodule



module two_bit_adder_TB();
    logic a0, a1, b0, b1, cin, s0, s1, c;
    two_bit_adder dut(a0, a1, b0, b1, cin, s0, s1, c);
    initial begin
        a0 = 0; a1 = 0; b0 = 0; b1 = 0; cin = 0; #10;
        a0 = 1;                                  #10;
        a0 = 0; a1 = 1;                          #10;
        a0 = 1; a1 = 0; b0 = 1;                  #10;
        a0 = 0; a1 = 1; b0 = 0; b1 = 1;          #10;
                                b1 = 0; cin = 1; #10;
        a0 = 1; a1 = 0; b0 = 1;                  #10;
                a1 = 1;         b1 = 1;          #10;
     end
endmodule



module LabCalculator_TB();
    logic c, d, a, b, y, z;
    LabCalculator dut(c, d, a, b, y, z);
    initial begin
        c = 0; d = 0; a = 0; b = 0; #10;
                             b = 1; #10;
                      a = 1; b = 0; #10;
                             b = 1; #10;
               d = 1; a = 0; b = 0; #10;
                             b = 1; #10;
                      a = 1; b = 0; #10;
                             b = 1; #10;
        c = 1; d = 0; a = 0; b = 0; #10;
                             b = 1; #10;
                      a = 1; b = 0; #10;
                             b = 1; #10;
               d = 1; a = 0; b = 0; #10;
                             b = 1; #10;
                      a = 1; b = 0; #10;
                             b = 1; #10;                         
    end
endmodule



module LabCalc_Sol2_TB();  
    logic c, d, a, b, y, z;
    LabCalc_Sol2 dut(c, d, a, b, y, z);
    initial begin
        c = 0; d = 0; a = 0; b = 0; #10;
                             b = 1; #10;
                      a = 1; b = 0; #10;
                             b = 1; #10;
               d = 1; a = 0; b = 0; #10;
                             b = 1; #10;
                      a = 1; b = 0; #10;
                             b = 1; #10;
        c = 1; d = 0; a = 0; b = 0; #10;
                             b = 1; #10;
                      a = 1; b = 0; #10;
                             b = 1; #10;
               d = 1; a = 0; b = 0; #10;
                             b = 1; #10;
                      a = 1; b = 0; #10;
                             b = 1; #10;                         
    end
endmodule
