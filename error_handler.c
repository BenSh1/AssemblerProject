#include "error_handler.h"
/*
 * prints errors to stdout
 * error_num - number of error to print
 * file_n - current file name
 * line_num - error line number
 * token - the string of the error
 */
void print_error(const int error_num, const char * file_n, const int line_num, const char * token) 
{
    printf("%s, Line: %-3d - Error: ", file_n, line_num);

    switch (error_num) {
        case LABEL_INVALID_LENGTH:      printf("Label exceeds allowed length of %d", MAX_LABEL_LENGTH - 1); break;
        case LINE_INVALID_LENGTH:       printf("Line exceeds allowed length of %d", MAX_LINE_LENGTH - 1); break;
        case LABEL_INVALID_INITIAL:     printf("Invalid label initial"); break;
        case LABEL_INVALID:             printf("Invalid label syntax"); break;
        case LABEL_NO_ENDING_COLON:     printf("Label no ending colon"); break;
        case DIRECTIVE_UNRECOGNIZED:    printf("Unrecognized directive"); break;
        case EXTRANEOUS_OPERAND:        printf("Extraneous operand"); break;
        case MISPLACED_COMMA_START:     printf("Misplaced comma before parameters"); break;
        case MISPLACED_COMMA_END:       printf("Misplaced comma at end of command"); break;
        case MISSING_PARAM:             printf("Missing parameter"); break;
        case CONSECUTIVE_COMMA:         printf("Consecutive comma"); break;
        case INVALID_PARAM:             printf("Invalid parameter"); break;
        case NUM_OUT_OF_RANGE:          printf("Number out of range"); break;
        case OUT_OF_MEMORY:             printf("Could not allocate memory"); break;
        case INVALID_STRING:            printf("Invalid string ,Remember all letters need to be inside quote!"); break;
        case STRING_INVALID_LENGTH:     printf("String exceeds allowed length"); break;
	case LABEL_EQUAL_TO_SAVE_WORD:  printf("Cannot have a label named on a saved word"); break;
	case LABEL_WITHOUT_DIRECTIVE_OR_INSTRUCTION:  printf("Cannot have a label without directive or instruction word"); break;
	case LABEL_ALREADY_DEFINED:     printf("Cannot have a label because it is already defined"); break;
	case NAME_FUNCTION_UNRECOGNIZE: printf("Unrecognized instruction name"); break;
	case INVALID_REGISTER:          printf("Invalid register "); break;
	case MISSING_COMMA:             printf("Missing comma "); break;
	case SET_ENTRY_TO_LABEL_UNDEFINED:  printf("Can't set entry attribute to label undefined"); break; 
	case COMMAND_WRONG_ORDER:  	printf("Command wrong order,command should be in another sequence"); break;
	case INVALID_COMMA:             printf("Invalid comma"); break;
	case STRING_NOT_IN_QUOTE:             printf("String not in quote"); break;
        default:                        printf("make print in as_parsing"); break;

    } 

    if (token == NULL || error_num == LABEL_INVALID_LENGTH || error_num == LINE_INVALID_LENGTH)
        printf(" \n");
    else    
        printf(" <%s>\n", token);
}
