function [C,A,nument] =  sdpasparse(C1,A1,K)
    funcprot(0);
    quot = size(A1,1)/size(A1,2);
    
    
   [ij,v,mn]=spget(sparse(A1));
   [ij2,v2,mn2] = spget(sparse(C1));
       //For the constr matrix
    for i = 1:size(ij,1)
       Elements(i) = full(A1(ij(i,1),ij(i,2)));
    end
    //For the obj function
    for i = 1:size(ij2,1)
       ObjEl(i) = full(C1(ij2(i,1),ij2(i,2)));
    end
    //For the constr matrix
    j = 0;
    for i = 1:size(ij2,1)
        objiind(i) =   ij2(i,1);
        if objiind(i) == 0
            iindices(i) = mn2(2);
        end 
               
        objjind(i) = ij2(i,2);
    end
    //For the obj function
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
     //For both the constr matrix and obj function
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

for i = 1:length(objiind)
   for j = 1:length(K)
       if objjind(i) <=ksum(j)
          blockno2(i) = j;
          break;
       end
   end

end


k = 1;
j= 1;
l= 1;
    
    for i = 1:length(K)*quot
        nument(i) = nnz(A1(k:k-1+K(j),:));
        k = k+K(j);
         j = j+1;
        if k >ksum(length(ksum))*l
            k = l*size(A1,2)+ 1;
            j = 1;
            l= l+1;
        end
    end

ksum = [0 ;ksum];
for i = 1:length(iindices)
    jindices(i) = jindices(i) - ksum(blockno(i));
    iindices(i) = iindices(i) - ksum(blockno(i));
            
end

for i = 1:length(objiind)
    objjind(i) = objjind(i) - ksum(blockno2(i));
    objiind(i) = objiind(i) - ksum(blockno2(i));
            
end

A = [constr blockno iindices jindices Elements]

C = [zeros(length(objiind),1) blockno2 objiind objjind ObjEl];


        
    
endfunction
