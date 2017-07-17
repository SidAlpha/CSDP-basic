function A =  sdpasparse(A1,K)
    funcprot(0);
    quot = size(A1,1)/size(A1,2);
   [ij,v,mn]=spget(A1);
    for i = 1:size(ij,1)
       Elements(i) = full(A1(ij(i,1),ij(i,2)));
    end
    j = 0;
    for i = 1:size(ij,1)
        j = floor(ij(i,1)/mn(2));
        iindices(i) =   ij(i,1) -j*mn(2);
        if iindices(i) == 0
            iindices(i) = mn(2);
        end 
        if modulo(ij(i,1),mn(2)) == 0
            constr(i) = j;
        else
            constr(i) = j+1;
        end
        
        jindices(i) = ij(i,2);
    end
    
    
   k = 0;
   for i = 1:length(K)
       ksum(i) = k+K(i);
       k = k+K(i);
   end
   
   for i = 1:length(iindices)
       for j = 1:length(K)
           if jindices(i) <=ksum(j)
              blockno(i) = j;
              break;
           end
       end

end
ksum = [0 ;ksum];
for i = 1:length(iindices)
    jindices(i) = jindices(i) - ksum(blockno(i));
    iindices(i) = iindices(i) - ksum(blockno(i));
            
end

A = [constr blockno iindices jindices Elements];



    


             
    
endfunction
