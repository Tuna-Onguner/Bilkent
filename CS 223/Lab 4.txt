//
// Bilkent University
// 2021-2022, Spring 
//
// CS 223
// Lab 4
//
// Deniz Tuna Onguner
// ID: 22001788
//


// FSM - traffic lights main module
module TrafficLights(input logic clk, reset, sa, sb, 
                    output logic [2:0] la, lb);

    typedef enum logic [2:0] {s0, s1, s2, s3, s4, s5, s6, s7} Statetype;
    Statetype [1:0] state, nextstate;
    
    parameter g = 3'b011;
    parameter y = 3'b001;
    parameter r = 3'b111;
    
    always_ff @(posedge clk, posedge reset)
        if(reset) state <= s0;
        else state <= nextstate;
    
    always_comb
    case(state)
        s0: if(sb) nextstate = s1;
            else   nextstate = s0;
        s1:        nextstate = s2;
        s2:        nextstate = s3;
        s3:        nextstate = s4;
        s4: if(sa) nextstate = s5;
            else   nextstate = s4;
        s5:        nextstate = s6;
        s6:        nextstate = s7;
        s7:        nextstate = s0;
        default:   nextstate = s0;
    endcase
    
    always_comb
    case(state)
        s0:      {la, lb} = {g, r};
        s1:      {la, lb} = {y, r};
        s2:      {la, lb} = {r, r};
        s3:      {la, lb} = {r, y};
        s4:      {la, lb} = {r, g};
        s5:      {la, lb} = {r, y};
        s6:      {la, lb} = {r, r};
        s7:      {la, lb} = {y, r};
        default: {la, lb} = {g, r};
    endcase 
endmodule



// Timing Clock Module
module Clock(input clk, output sclk);
    logic [31:0] timer = 0;
    logic out;
    
    always @(posedge clk)
        begin
        timer <= timer + 1;
        if (timer == 150000000)
            begin
                timer <= 0;
                out = ~out;
            end
        end
    assign sclk = out;
endmodule



// Testbench
module TestBench();
    logic clk, reset, sa, sb;
    logic [2:0] la, lb;
    TrafficLights dut(clk, reset, sa, sb, la, lb);
    initial begin
        reset = 0; 
        //sa = 0; sb = 1; #10;
        //sa = 1; sb = 0; #10;
        //sa = 0; sb = 0; #10;
        //sa = 1; sb = 1; #10;
                           
    end
                                    
    always
        begin
        clk <= 1; #10;
        clk <= 0; #10;
        end
endmodule
