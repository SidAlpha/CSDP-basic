function  semidefi(C1,A1,b,K)
    funcprot(0)
    
    [C,A,nument] =  sdpasparse(C1,A1,K);
    printf("Entering CPP\n")
    a = csdp(C',A',b,K,nument);
    
endfunction
