#include <assert.h>
#include <string.h>

#include "grug_main.h"
#include "grug_arena.h"
#include "grug_options.h"

// SECTION MARK: mini lib

// TODO: add a way to have a user-defined realloc, and fall back to this otherwise
void* grug_realloc(void* ptr, size_t old_len, size_t new_len) {
	if(!ptr) {

	}
	assert(new_len >= old_len);
	if(new_len == old_len) {
		return ptr;
	}
	void* new_ptr = GRUG_MALLOC(new_len);
	if(!new_ptr) {
		return 0;
	}
	memcpy(new_ptr, ptr, old_len);
	GRUG_FREE(ptr, old_len);
	return new_ptr;
}

// SECTION MARK: function implementations

struct grug_init_settings grug_default_settings(void) {
	return (struct grug_init_settings){ 
		// TODO: interpreter backend
		.mod_api_path = (struct grug_string){.len = 0},
		.mods_dir_path = (struct grug_string){.len = 0},
		.runtime_error_handler = (struct grug_runtime_error_handler){0},
		.backend = (struct grug_backend){.obj = 0, .vtable = 0},
	};
}

struct grug_state* grug_init(struct grug_init_settings settings) {
	(void) settings;
	// TODO: implement
	return 0;
}

void grug_swap_backend(struct grug_state* gst, struct grug_backend backend) {
	(void)gst;
	(void)backend;
	// TODO: implement
}

void grug_set_fast_mode(struct grug_state* gst, bool fast) {
	(void)gst;
	(void)fast;
	// TODO: implement
}
bool grug_register_game_fn(struct grug_state* gst, char const* game_fn_name, game_fn fn) {
	(void)gst;
	(void)game_fn_name;
	(void)fn;
	// TODO: implement
	return false;
}

struct grug_on_fns grug_get_fn_ids(struct grug_state* gst) {
	(void)gst;
	// TODO: implement
	return (struct grug_on_fns){
		.count = 0,
		.entries = 0,
	};
}

const struct grug_mod_dir* grug_get_mods(struct grug_state* gst) {
	(void)gst;
	// TODO: implement
	return 0;
}

grug_id grug_create_entity(struct grug_state* gst, grug_file_id script, grug_object_id me_id) {
	(void)gst;
	(void)script;
	(void)me_id;
	// TODO: implement
	return 0;
}

grug_file_id grug_entity_get_file(struct grug_state* gst, grug_id entity) {
	(void)gst;
	(void)entity;
	// TODO: implement
	return 0;
}

struct grug_entity* grug_entity_get_data(struct grug_state* gst, grug_id entity) {
	(void)gst;
	(void)entity;
	// TODO: implement
	return 0;
}

void grug_deinit_entity(struct grug_state* gst, grug_id entity) {
	(void)gst;
	(void)entity;
	// TODO: implement
}

struct grug_updates_list grug_update(struct grug_state* gst) {
	(void)gst;
	// TODO: implement
	return (struct grug_updates_list) {
		.count = 0,
		.updates = 0,
	};
}

void grug_deinit(struct grug_state* gst) {
	(void)gst;
	// TODO: implement
}

bool grug_all_game_functions_registered(struct grug_state* gst) {
	(void) gst;
	// TODO: implement
	return false;
}

bool grug_call_on_function(struct grug_state* gst, grug_id entity, grug_on_fn_id on_fn_id, union grug_value* args, size_t args_len) {
	(void)gst;
	(void)on_fn_id;
	(void)entity;
	(void)args;
	(void)args_len;
	// TODO: implement
	return false;
}

bool grug_call_on_function_raw(struct grug_state* gst, grug_id entity, grug_on_fn_id on_fn_id, union grug_value* args) {
	(void)gst;
	(void)on_fn_id;
	(void)entity;
	(void)args;
	// TODO: implement
	return false;
}

void grug_backend_call(struct grug_state* gst, grug_on_fn_id fn, grug_id entity, const union grug_value args[]) {
	(void)gst;
	(void)fn;
	(void)entity;
	(void)args;
	// TODO: implement
}

struct grug_string grug_alloc_string(size_t len) {
	if(len == 0) {
		return (struct grug_string){
			.ptr = 0,
			.len = 0,
		};
	}
	return (struct grug_string) {
		.ptr = GRUG_MALLOC(len + 1),
		.len = len,
	};
}

void grug_free_string(struct grug_string str) {
	if(str.len > 0) {
		GRUG_FREE(str.ptr, str.len + 1);
	}
}

void grug_free_tokens(struct grug_tokens tokens) {
	for(size_t token_index = 0; token_index < tokens.tokens_len; token_index += 1) {
		grug_free_string(tokens.tokens[token_index].contents);
	}
	GRUG_FREE(tokens.tokens, tokens.tokens_len * sizeof(struct grug_token));
}

void grug_free_ast(struct grug_ast ast)  {
	grug_arena_deinit(&ast.arena);
}

static void add_token(struct grug_tokens* tokens, size_t* capacity, struct grug_token token) {
	if(*capacity == tokens->tokens_len) {
		size_t new_capacity = *capacity * 3 / 2 + 1;
		struct grug_token* new_tokens = grug_realloc(tokens->tokens, *capacity, new_capacity);
		assert(new_tokens);
		*capacity = new_capacity;
		tokens->tokens = new_tokens;
	}
	tokens->tokens[tokens->tokens_len] = token;
	tokens->tokens_len += 1;
}

struct grug_tokens grug_to_tokens(struct grug_string grug, struct grug_error* o_error) {
	(void)o_error;
	struct grug_tokens tokens = {
		.tokens = 0,
		.tokens_len = 0,
	};
	size_t tokens_capacity;
	char* src = grug.ptr;
	size_t src_len = grug.len;
	size_t i = 0;
	// TODO: clean up this horrible copy-paste of the grug-for-python code unceremoniously translated line-for-line without splitting things into functions.
	while(i < grug.len) {
		char c = src[i];
		if(c == '(') {
			add_token(&tokens, &tokens_capacity, (struct grug_token){.type = GRUG_TOKEN_OPEN_PARENTHESIS, {.len = 0}});
			i += 1;
		}
		else if(c == ')') {
			add_token(&tokens, &tokens_capacity, (struct grug_token){.type = GRUG_TOKEN_CLOSE_PARENTHESIS, .contents = {.ptr = &src[i], .len = 1}});
			i += 1;
		}
		else if(c == '{') {
			add_token(&tokens, &tokens_capacity, (struct grug_token){.type = GRUG_TOKEN_OPEN_BRACE, .contents = {.ptr = &src[i], .len = 1}});
			i += 1;
		}
		else if(c == '}') {
			add_token(&tokens, &tokens_capacity, (struct grug_token){.type = GRUG_TOKEN_CLOSE_BRACE, .contents = {.ptr = &src[i], .len = 1}});
			i += 1;
		}
		else if(c == '+') {
			add_token(&tokens, &tokens_capacity, (struct grug_token){.type = GRUG_TOKEN_PLUS, .contents = {.ptr = &src[i], .len = 1}});
			i += 1;
		}
		else if(c == '-') {
			add_token(&tokens, &tokens_capacity, (struct grug_token){.type = GRUG_TOKEN_MINUS, .contents = {.ptr = &src[i], .len = 1}});
			i += 1;
		}
		else if(c == '*'){
			add_token(&tokens, &tokens_capacity, (struct grug_token){.type = GRUG_TOKEN_MULTIPLICATION, .contents = {.ptr = &src[i], .len = 1}});
			i += 1;
		}
		else if(c == '/') {
			add_token(&tokens, &tokens_capacity, (struct grug_token){.type = GRUG_TOKEN_DIVISION, .contents = {.ptr = &src[i], .len = 1}});
			i += 1;
		}
		else if(c == ',') {
			add_token(&tokens, &tokens_capacity, (struct grug_token){.type = GRUG_TOKEN_COMMA, .contents = {.ptr = &src[i], .len = 1}});
			i += 1;
		}
		else if(c == ':') {
			add_token(&tokens, &tokens_capacity, (struct grug_token){.type = GRUG_TOKEN_COLON, .contents = {.ptr = &src[i], .len = 1}});
			i += 1;
		}
		else if(c == '\n') {
			add_token(&tokens, &tokens_capacity, (struct grug_token){.type = GRUG_TOKEN_NEWLINE, .contents = {.ptr = &src[i], .len = 1}});
			i += 1;
		}
		else if(c == '=' && i + 1 < src_len && src[i + 1] == '=') {
			add_token(&tokens, &tokens_capacity, (struct grug_token){.type = GRUG_TOKEN_EQUALS, .contents = GRUG_WRAP_STRING("==")});
			i += 2;
		}
		else if(c == '!' && i + 1 < src_len && src[i + 1] == '=') {
			add_token(&tokens, &tokens_capacity, (struct grug_token){.type = GRUG_TOKEN_NOT_EQUALS, .contents = GRUG_WRAP_STRING("!=")});
			i += 2;
		}
		else if(c == '=') {
			add_token(&tokens, &tokens_capacity, (struct grug_token){.type = GRUG_TOKEN_ASSIGNMENT, .contents = {.ptr = &src[i], .len = 1}});
			i += 1;
		}
		else if(c == '>' && i + 1 < src_len && src[i + 1] == '=') {
			add_token(&tokens, &tokens_capacity, (struct grug_token){.type = GRUG_TOKEN_GREATER_OR_EQUAL, .contents = GRUG_WRAP_STRING(">=")});
			i += 2;
		}
		else if(c == '>') {
			add_token(&tokens, &tokens_capacity, (struct grug_token){.type = GRUG_TOKEN_GREATER, .contents = GRUG_WRAP_STRING(">")});
			i += 1;
		}
		else if(c == '<' && i + 1 < src_len && src[i + 1] == '=') {
			add_token(&tokens, &tokens_capacity, (struct grug_token){.type = GRUG_TOKEN_LESS_OR_EQUAL, .contents = GRUG_WRAP_STRING("<=")});
			i += 2;
		}
		else if(c == '<') {
			add_token(&tokens, &tokens_capacity, (struct grug_token){.type = GRUG_TOKEN_LESS, .contents = GRUG_WRAP_STRING("<")});
			i += 1;
		}
		else if((i+sizeof("and") < src_len && memcmp(&src[i], "and", sizeof("and")) == 0) && ((i + 3) >= src_len || (!(src[i + 3] >= '0' && src[i + 3] <= '9') || src[i + 3] == '_'))) {
			add_token(&tokens, &tokens_capacity, (struct grug_token){.type = GRUG_TOKEN_AND, .contents = GRUG_WRAP_STRING("and")});
			i += 3;
		}
		else if((i+sizeof("or") < src_len && memcmp(&src[i], "or", sizeof("or")) == 0) && ((i + 2) >= src_len || (!(src[i + 2] >= '0' && src[i + 2] <= '9') || src[i + 2] == '_'))) {
			add_token(&tokens, &tokens_capacity, (struct grug_token){.type = GRUG_TOKEN_OR, .contents = GRUG_WRAP_STRING("or")});
			i += 2;
		}
		else if((i+sizeof("not") < src_len && memcmp(&src[i], "not", sizeof("not")) == 0) && ((i + 3) >= src_len || (!(src[i + 3] >= '0' && src[i + 3] <= '9') || src[i + 3] == '_'))) {
			add_token(&tokens, &tokens_capacity, (struct grug_token){.type = GRUG_TOKEN_NOT, .contents = GRUG_WRAP_STRING("not")});
			i += 3;
		}
		else if((i+sizeof("true") < src_len && memcmp(&src[i], "true", sizeof("true")) == 0) && ((i + 4) >= src_len || (!(src[i + 4] >= '0' && src[i + 4] <= '9') || src[i + 4] == '_'))) {
			add_token(&tokens, &tokens_capacity, (struct grug_token){.type = GRUG_TOKEN_TRUE, .contents = GRUG_WRAP_STRING("true")});
			i += 4;
		}
		else if((i+sizeof("false") < src_len && memcmp(&src[i], "false", sizeof("false")) == 0) && ((i + 5) >= src_len || (!(src[i + 5] >= '0' && src[i + 5] <= '9') || src[i + 5] == '_'))) {
			add_token(&tokens, &tokens_capacity, (struct grug_token){.type = GRUG_TOKEN_FALSE, .contents = GRUG_WRAP_STRING("false")});
			i += 5;
		}
		else if((i+sizeof("if") < src_len && memcmp(&src[i], "if", sizeof("if")) == 0) && ((i + 2) >= src_len || (!(src[i + 2] >= '0' && src[i + 2] <= '9') || src[i + 2] == '_'))) {
			add_token(&tokens, &tokens_capacity, (struct grug_token){.type = GRUG_TOKEN_IF, .contents = GRUG_WRAP_STRING("if")});
			i += 2;
		}
		else if((i+sizeof("else") < src_len && memcmp(&src[i], "else", sizeof("else")) == 0) && ((i + 4) >= src_len || (!(src[i + 4] >= '0' && src[i + 4] <= '9') || src[i + 4] == '_'))) {
			add_token(&tokens, &tokens_capacity, (struct grug_token){.type = GRUG_TOKEN_ELSE, .contents = GRUG_WRAP_STRING("else")});
			i += 4;
		}
		else if((i+sizeof("while") < src_len && memcmp(&src[i], "while", sizeof("while")) == 0) && ((i + 5) >= src_len || (!(src[i + 5] >= '0' && src[i + 5] <= '9') || src[i + 5] == '_'))) {
			add_token(&tokens, &tokens_capacity, (struct grug_token){.type = GRUG_TOKEN_WHILE, .contents = GRUG_WRAP_STRING("while")});
			i += 5;
		}
		else if((i+sizeof("break") < src_len && memcmp(&src[i], "break", sizeof("break")) == 0) && ((i + 5) >= src_len || (!(src[i + 5] >= '0' && src[i + 5] <= '9') || src[i + 5] == '_'))) {
			add_token(&tokens, &tokens_capacity, (struct grug_token){.type = GRUG_TOKEN_BREAK, .contents = GRUG_WRAP_STRING("break")});
			i += 5;
		}
		else if((i+sizeof("return") < src_len && memcmp(&src[i], "return", sizeof("return")) == 0) && ((i + 6) >= src_len || (!(src[i + 6] >= '0' && src[i + 6] <= '9') || src[i + 6] == '_'))) {
			add_token(&tokens, &tokens_capacity, (struct grug_token){.type = GRUG_TOKEN_RETURN, .contents = GRUG_WRAP_STRING("return")});
			i += 6;
		}
		else if((i+sizeof("continue") < src_len && memcmp(&src[i], "continue", sizeof("continue")) == 0) && ((i + 8) >= src_len || (!(src[i + 8] >= '0' && src[i + 8] <= '9') || src[i + 8] == '_'))) {
			add_token(&tokens, &tokens_capacity, (struct grug_token){.type = GRUG_TOKEN_CONTINUE, .contents = GRUG_WRAP_STRING("continue")});
			i += 8;
		}
	}

	// 	elif c == " ":
	// 		if i + 1 >= src_len or src[i + 1] != " ":
	// 			tokens.append(Token(TokenType.SPACE_TOKEN, " "))
	// 			i += 1
	// 			continue

	// 		old_i = i
	// 		while i < src_len and src[i] == " ":
	// 			i += 1

	// 		spaces = i - old_i

	// 		if spaces % SPACES_PER_INDENT != 0:
	// 			raise TokenizerError(
	// 				f"Encountered {spaces} spaces, while indentation expects multiples of {SPACES_PER_INDENT} spaces, on line {self.get_character_line_number(i)}"
	// 			)

	// 		tokens.append(Token(TokenType.INDENTATION_TOKEN, " " * spaces))
	// 	elif c == '"':
	// 		open_quote_index = i
	// 		i += 1
	// 		start = i
	// 		while i < src_len and src[i] != '"':
	// 			if src[i] == "\0":
	// 				raise TokenizerError(
	// 					f"Unexpected null byte on line {self.get_character_line_number(i)}"
	// 				)
	// 			elif src[i] == "\\" and i + 1 < src_len and src[i + 1] in "\r\n":
	// 				raise TokenizerError(
	// 					f"Unexpected line break in string on line {self.get_character_line_number(i)}"
	// 				)
	// 			i += 1
	// 		if i >= src_len:
	// 			raise TokenizerError(
	// 				f'Unclosed " on line {self.get_character_line_number(open_quote_index)}'
	// 			)
	// 		tokens.append(Token(TokenType.STRING_TOKEN, src[start:i]))
	// 		i += 1
	// 	elif c.isalpha() or c == "_":
	// 		start = i
	// 		while i < src_len and (src[i].isalnum() or src[i] == "_"):
	// 			i += 1
	// 		tokens.append(Token(TokenType.WORD_TOKEN, src[start:i]))
	// 	elif c.isdigit():
	// 		start = i
	// 		seen_period = False
	// 		i += 1
	// 		while i < src_len and (src[i].isdigit() or src[i] == "."):
	// 			if src[i] == ".":
	// 				if seen_period:
	// 					raise TokenizerError(
	// 						f"Encountered two '.' periods in a number on line {self.get_character_line_number(i)}"
	// 					)
	// 				seen_period = True
	// 			i += 1

	// 		if src[i - 1] == ".":
	// 			raise TokenizerError(
	// 				f"Missing digit after decimal point in '{src[start:i]}'"
	// 			)

	// 		tokens.append(Token(TokenType.NUMBER_TOKEN, src[start:i]))
	// 	elif c == "#":
	// 		i += 1
	// 		if i >= src_len or src[i] != " ":
	// 			raise TokenizerError(
	// 				f"Expected a single space after the '#' on line {self.get_character_line_number(i)}"
	// 			)
	// 		i += 1
	// 		start = i
	// 		while i < src_len and src[i] not in "\r\n":
	// 			if src[i] == "\0":
	// 				raise TokenizerError(
	// 					f"Unexpected null byte on line {self.get_character_line_number(i)}"
	// 				)
	// 			i += 1

	// 		comment_len = i - start
	// 		if comment_len == 0:
	// 			raise TokenizerError(
	// 				f"Expected the comment to contain some text on line {self.get_character_line_number(i)}"
	// 			)

	// 		if src[i - 1].isspace():
	// 			raise TokenizerError(
	// 				f"A comment has trailing whitespace on line {self.get_character_line_number(i)}"
	// 			)

	// 		tokens.append(Token(TokenType.COMMENT_TOKEN, src[start:i]))
	// 	else:
	// 		raise TokenizerError(
	// 			f"Unrecognized character '{c}' on line {self.get_character_line_number(i)}"
	// 		)

	return (struct grug_tokens){0};
}

struct grug_ast tokens_to_ast(struct grug_tokens tokens, struct grug_error* o_error) {
	(void)tokens;
	(void)o_error;
	// TODO
	return (struct grug_ast){.helper_functions_count = 0, .on_functions_count = 0, .members_count = 0};
}

struct grug_tokens ast_to_tokens(struct grug_ast ast, struct grug_error* o_error) {
	(void)ast;
	(void)o_error;
	// TODO
	return (struct grug_tokens){.tokens_len = 0};
}

struct grug_string tokens_to_grug(struct grug_tokens tokens, struct grug_error* o_error) {
	(void)tokens;
	(void)o_error;
	// TODO
	return (struct grug_string){.len = 0};
}

struct grug_ast json_to_ast(struct grug_string json, struct grug_error* o_error) {
	(void)json;
	(void)o_error;
	// TODO
	return (struct grug_ast){.helper_functions_count = 0, .on_functions_count = 0, .members_count = 0};
}

struct grug_string ast_to_json(struct grug_ast ast, struct grug_error* o_error) {
	(void)ast;
	(void)o_error;
	// TODO
	return (struct grug_string){.len = 0};
}

struct grug_ast grug_to_ast(struct grug_string grug, struct grug_error* o_error) {
	(void)grug;
	(void)o_error;
	// TODO
	return (struct grug_ast){.helper_functions_count = 0, .on_functions_count = 0, .members_count = 0};
}

struct grug_string ast_to_grug(struct grug_ast ast, struct grug_error* o_error) {
	(void)ast;
	(void)o_error;
	// TODO
	return (struct grug_string){.len = 0};
}

struct grug_string grug_to_json(struct grug_string grug, struct grug_error* o_error) {
	(void)grug;
	(void)o_error;
	// TODO
	return (struct grug_string){.len = 0};
}

struct grug_string json_to_grug(struct grug_string json, struct grug_error* o_error) { 
	(void)json;
	(void)o_error;
	// TODO
	return (struct grug_string){.len = 0};
}
