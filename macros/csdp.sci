function  csdp(C,A1,b,K)
    funcprot(0)
    A =  sdpasparse(A1,K);
    a = scilab_io(C,A,b,K);
    
endfunction
