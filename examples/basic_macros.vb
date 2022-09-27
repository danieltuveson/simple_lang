

function my_function(x, y, z)

end function

if things then 
    stuff
end if 

template else_if(IF, condition, THEN, NEWLINE, if_statements, NEWLINE, ELSE, NEWLINE, else_statements, END, IF, NEWLINE | EOF)
    dim result
    result = condition
    if result then
        if_statements
    end if 
    if not result then 
        else_statements
    end if 
end macro

macro for(args)
    
    if len(arguments) <> 3 then 
        error("for loop requires 3 arguments")
    end if 
    ###
    dim init, condition, update
    init = {arguments[0]}
    condition = {arguments[1]}
    update = fun()
        {arguments[2]}
    end fun
    while (condition)
        {body}
    end while
    ###
end macro

macro backward_assign(to_assign, var)
    
end macro


' first class functions
dim thing
thing = function(some, stuff) 

end function



template module(args)
    dim module_name, stmts, stmt
    module_name = consume_tok(args[0])
    stmts = consume_statements(args[1])

    for each stmt in stmts
        if not is_function(stmt) then 
            error("module may only contain function definitions")
        end if
    next
end template

































