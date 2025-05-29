// =============================================================================
// File        : wbDualPortBRAM.v
// Author      : @fjpolo
// email       : fjpolo@gmail.com
// Description : <Brief description of the module or file>
// License     : MIT License
//
// Copyright (c) 2025 | @fjpolo
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// =============================================================================
// wbDualPortBRAM.sv
// Very Basic Dual-Port Block RAM as a Wishbone B4 Pipelined Slave
//
// This module implements a single Wishbone slave interface that accesses an
// internal memory array. The "dual-port" aspect here refers to the internal
// memory structure, which in real FPGAs would be inferred as a dual-port
// Block RAM, allowing for concurrent read/write operations that are
// beneficial for pipelined Wishbone access.
//
// The Wishbone B4 Pipelined protocol means:
// - Inputs (adr, dat_i, sel, we, stb, cyc) are registered.
// - Outputs (ack, dat_o) are asserted one cycle after inputs if valid.
// - Read data (dat_o) is valid in the same cycle as ack_o.
`default_nettype none
`timescale 1ps/1ps

module wbDualPortBRAM #( // Module name remains wb_dual_port_bram
    parameter ADDR_WIDTH = 7,  // Address width for the RAM (e.g., 7 for 128 words)
    parameter DATA_WIDTH = 32,  // Data width of each word (e.g., 32 bits)
    localparam MEM_SIZE   = (1 << ADDR_WIDTH) // Total number of words in RAM
) (
    input wire                            i_clk, // System clock
    input wire                            i_rst, // System reset

    // Wishbone Slave Interface (B4 Pipelined)
    input wire                            i_wb_cyc,   // Cycle valid
    input wire                            i_wb_stb,   // Strobe (request valid)
    input wire                            i_wb_we,    // Write enable
    input wire [ADDR_WIDTH-1:0]           i_wb_adr,   // Address
    input wire [DATA_WIDTH-1:0]           i_wb_dat,   // Write data
    input wire [DATA_WIDTH/8-1:0]         i_wb_sel,   // Byte select (for partial word writes)

    output reg                            o_wb_ack,   // Acknowledge
    output reg [DATA_WIDTH-1:0]           o_wb_dat,   // Read data
    output reg                            o_wb_err    // Error (always 0 for this basic RAM)
);

    // Internal memory array
    logic [DATA_WIDTH-1:0] mem [MEM_SIZE-1:0];

    // Registered Wishbone inputs for pipelining
    logic        _i_wb_cyc_r;
    logic        _i_wb_stb_r;
    logic        _i_wb_we_r;
    logic [ADDR_WIDTH-1:0] _i_wb_adr_r;
    logic [DATA_WIDTH-1:0] _i_wb_dat_r;
    logic [DATA_WIDTH/8-1:0] _i_wb_sel_r;

    // Register for next acknowledge cycle
    logic        _o_wb_ack_next;

    // No errors for a simple RAM
    assign o_wb_err = 1'b0;

    always_ff @(posedge i_clk or posedge i_rst) begin
        if (i_rst) begin
            _i_wb_cyc_r <= 1'b0;
            _i_wb_stb_r <= 1'b0;
            _i_wb_we_r  <= 1'b0;
            _i_wb_adr_r <= '0;
            _i_wb_dat_r <= '0;
            _i_wb_sel_r <= '0;
            o_wb_ack    <= 1'b0;
            o_wb_dat    <= '0;
        end else begin
            // Stage 1: Register Wishbone inputs
            _i_wb_cyc_r <= i_wb_cyc;
            _i_wb_stb_r <= i_wb_stb;
            _i_wb_we_r  <= i_wb_we;
            _i_wb_adr_r <= i_wb_adr;
            _i_wb_dat_r <= i_wb_dat;
            _i_wb_sel_r <= i_wb_sel;

            // Stage 2: Handle memory access and acknowledge
            // o_wb_ack is asserted based on previous cycle's stb/cyc
            _o_wb_ack_next <= i_wb_stb && i_wb_cyc;

            // Wishbone Write operation (uses registered inputs)
            if (_i_wb_stb_r && _i_wb_cyc_r && _i_wb_we_r) begin
                // Apply byte enables for write
                for (int i = 0; i < DATA_WIDTH/8; i++) begin
                    if (_i_wb_sel_r[i]) begin
                        mem[_i_wb_adr_r][(i*8) +: 8] <= _i_wb_dat_r[(i*8) +: 8];
                    end
                end
            end

            // Wishbone Read operation (uses registered inputs)
            // Read data is based on the address from the previous cycle (pipelined)
            // Data is outputted when ack_o is asserted.
            if (_i_wb_stb_r && _i_wb_cyc_r && !_i_wb_we_r) begin
                o_wb_dat <= mem[_i_wb_adr_r];
            end else if (!(_i_wb_stb_r && _i_wb_cyc_r)) begin
                // Clear output if no active Wishbone transaction for read
                o_wb_dat <= '0;
            end

            // Update acknowledge output
            o_wb_ack <= _o_wb_ack_next;
        end
    end

endmodule
