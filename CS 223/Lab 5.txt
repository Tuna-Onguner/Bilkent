//
// Bilkent University
// 2021-2022, Spring 
//
// CS 223
// Lab 5
//
// Deniz Tuna Onguner
// ID: 22001788
//


module Main(input logic CLK, RESET, Load, Shift, [7:0] A, B,
           output logic [7:0] sum);
    logic slowedClock;
    Clock clock(CLK, slowedClock);
    SerialAdder SA(slowedClock, RESET, Load, Shift, A, B, sum);
endmodule



module Clock(input clk, output sclk);
    logic [31:0] timer = 0;
    logic out;
    
    always @(posedge clk)
        begin
        timer <= timer + 1;
        if (timer == 50000000)
            begin
                timer <= 0;
                out = ~out;
            end
        end
    assign sclk = out;
endmodule



module SerialAdder(input logic CLK, RESET, Load, Shift, [7:0] A, B,
                  output logic [7:0] sum);
    logic [7:0] QA, QB;
    logic a, b, cin, cout, s, x;
    ShifterRight SA(CLK, RESET, Load, Shift, 1'b0, A, a, QA);
    ShifterRight SB(CLK, RESET, Load, Shift, 1'b0, B, b, QB);
    FullAdder FA(a, b, CLK, Shift, cin, s, cout);
    ShifterRight SS(CLK, RESET, Load, Shift, s, 8'b00000000, x, sum);
    always_ff @(posedge CLK)
        if(cout == 1'b1) cin <= 1;
        else             cin <= 0;
endmodule



module ShifterRight(input logic CLK, RESET, Load, Shift, s_in, [7:0] in,
                   output logic s_out, [7:0] Q);
    always_ff @(posedge CLK, posedge RESET)
        if(RESET)            Q <= 8'b00000000;
        else if(Load)        Q <= in;
        else if(Shift)
        begin
            if(s_in | ~s_in) Q <= {s_in, Q[7:1]};
        end
    assign s_out = Q[0];
endmodule



module FullAdder(input logic a, b, CLK, Shift, cin,
                output logic s, cout);
    logic p, g;
    
    always_comb
    begin
        p <= a ^ b;
        g <= a & b;

        s <= (p ^ cin);
        cout <= g | (p & cin);
    end
endmodule



module serialAdder_testbench();
    logic CLK, RESET, Load, Shift;
    logic [7:0] A, B, sum;
    SerialAdder SA(CLK, RESET, Load, Shift, A, B, sum);
    initial begin
        Shift = 1;
        RESET = 0;
        A = 8'b00000001;
        B = 8'b00000001;
                   Load = 0; #10;
                   Load = 1; #10;
                   Load = 0; #10;
                             #200; Shift = 0;
        
    end
    
    always
        begin
        CLK <= 0; #10;
        CLK <= 1; #10;
        end
endmodule



module shifterRight_testbench();
    logic CLK, RESET, Load, Shift, sout;
    logic [7:0] in, Q;
    ShifterRight dut(CLK, RESET, Load, Shift, 1'b0, in, sout, Q);
    initial begin
        in = 8'b10101010;
        RESET = 0; Shift = 1;
                   Load = 0; #10;
                   Load = 1; #10;
                   Load = 0; #10;
    end
    
    always
        begin
        CLK <= 0; #10;
        CLK <= 1; #10;
        end
endmodule
