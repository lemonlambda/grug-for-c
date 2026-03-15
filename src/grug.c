#include "grug.h"

#ifdef GRUG_MALLOC_HEADER
	#include GRUG_MALLOC_HEADER
#else
	#include <malloc.h>
#endif

#ifndef GRUG_MALLOC
	#define GRUG_MALLOC(_size) malloc(_size)
#endif

#ifndef GRUG_FREE
	#define GRUG_FREE(_ptr, _len) ((void)(_len), free(_ptr))
#endif

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

grug_id grug_create_entity(struct grug_state* gst, grug_file_id script) {
	(void)gst;
	(void)script;
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

static void free_type(struct grug_type t) {
	switch(t.type) {
		case GRUG_TYPE_ID:
			grug_free_string(t.extra_data.entity_type);
			break;
		case GRUG_TYPE_RESOURCE:
			grug_free_string(t.extra_data.resource_type);
			break;
		case GRUG_TYPE_ENTITY:
			grug_free_string(t.extra_data.custom_name);
			break;
		default:
			break;
	}
}

static void free_argument(struct grug_argument a) {
	grug_free_string(a.name);
	free_type(a.type);
}

static void free_expression(struct grug_expr expr) {
	free_type(expr.result_type);
	switch ((enum grug_expr_type_enum)expr.type) {
		case GRUG_EXPR_TYPE_TRUE: {
			break;
		}
		case GRUG_EXPR_TYPE_FALSE: {
			break;
		}
		case GRUG_EXPR_TYPE_STRING: {
			grug_free_string(expr.expr_data.string);
			break;
		}
		case GRUG_EXPR_TYPE_RESOURCE: {
			grug_free_string(expr.expr_data.resource);
			break;
		}
		case GRUG_EXPR_TYPE_ENTITY: {
			grug_free_string(expr.expr_data.entity);
			break;
		}
		case GRUG_EXPR_TYPE_IDENTIFIER: {
			grug_free_string(expr.expr_data.identifier_name);
			break;
		}
		case GRUG_EXPR_TYPE_NUMBER: {
			grug_free_string(expr.expr_data.number.string);
			break;
		}
		case GRUG_EXPR_TYPE_NOTHING: {
			break;
		}
		case GRUG_EXPR_TYPE_UNARY: {
			free_expression(*expr.expr_data.unary.inner);
			GRUG_FREE(expr.expr_data.unary.inner, sizeof(*expr.expr_data.unary.inner));
			break;
		}
		case GRUG_EXPR_TYPE_BINARY: {
			free_expression(*expr.expr_data.binary.left);
			free_expression(*expr.expr_data.binary.right);
			GRUG_FREE(expr.expr_data.binary.left, sizeof(*expr.expr_data.binary.left));
			GRUG_FREE(expr.expr_data.binary.right, sizeof(*expr.expr_data.binary.right));
			break;
		}
		case GRUG_EXPR_TYPE_CALL: {
			grug_free_string(expr.expr_data.call.function_name);
			for(size_t arg_index = 0; arg_index < expr.expr_data.call.args_count; arg_index += 1) {
				free_expression(expr.expr_data.call.args[arg_index]);
			}
			GRUG_FREE(expr.expr_data.call.args, expr.expr_data.call.args_count * sizeof(*expr.expr_data.call.args));
			break;
		}
		case GRUG_EXPR_TYPE_PARENTHESIZED: {
			free_expression(*expr.expr_data.parenthesized);
			GRUG_FREE(expr.expr_data.parenthesized, sizeof(*expr.expr_data.parenthesized));
			break;
		}
	}
}

// forward declaration
static void free_statement(struct grug_statement st);

static void free_block(struct grug_block b) {
	for(size_t statement_index = 0; statement_index < b.statements_len; statement_index += 1) {
		free_statement(b.statements[statement_index]);
	}
}

static void free_if(struct grug_if_branch branch) {
	free_expression(branch.cond);
	free_block(branch.block);
}

static void free_statement(struct grug_statement st) {
	switch((enum grug_statement_type_enum)st.type) {
		case GRUG_STATEMENT_VARIABLE: {
			grug_free_string(st.statement_data.variable.name);
			free_type(st.statement_data.variable.type);
			free_expression(st.statement_data.variable.assignment_expr);
			break;
		}
		case GRUG_STATEMENT_CALL: {
			free_expression(st.statement_data.call);
			break;
		}
		case GRUG_STATEMENT_IF: {
			free_if(st.statement_data.if_stmt.branch);
			for(size_t branch_index = 0; branch_index < st.statement_data.if_stmt.additional_branches_len; branch_index += 1) {
				free_if(st.statement_data.if_stmt.additional_branches[branch_index]);
			}
			GRUG_FREE(st.statement_data.if_stmt.additional_branches, st.statement_data.if_stmt.additional_branches_len * sizeof(struct grug_if_branch));
			free_block(st.statement_data.if_stmt.else_block);
			break;
		}
		case GRUG_STATEMENT_WHILE: {
			free_expression(st.statement_data.while_stmt.condition);
			free_block(st.statement_data.while_stmt.block);
			break;
		}
		case GRUG_STATEMENT_RETURN: {
			free_expression(st.statement_data.return_stmt.expr);
			break;
		}
		case GRUG_STATEMENT_COMMENT: {
			grug_free_string(st.statement_data.comment);
			break;
		}
		case GRUG_STATEMENT_BREAK: {
			break;
		}
		case GRUG_STATEMENT_CONTINUE: {
			break;
		}
		case GRUG_STATEMENT_EMPTY: {
			break;
		}
	}
}

static void free_on_fn(struct grug_on_function* fn) {
	grug_free_string(fn->name);
	for(size_t argument_index = 0; argument_index < fn->arguments_len; argument_index += 1) {
		free_argument(fn->arguments[argument_index]);
	}
	free_block(fn->block);
	GRUG_FREE(fn, sizeof(struct grug_on_function));
}

void grug_free_ast(struct grug_ast ast) {
	for(size_t member_index = 0; member_index < ast.members_count; member_index += 1) {
		struct grug_member_variable member = ast.members[member_index];
		grug_free_string(member.name);
	}

	for(size_t on_fn_index = 0; on_fn_index < ast.on_functions_count; on_fn_index += 1) {
		free_on_fn(ast.on_functions[on_fn_index]);
	}
}

struct grug_tokens grug_to_tokens(struct grug_string grug) {
	(void)grug;
	// TODO
	return (struct grug_tokens){.tokens_len = 0};
}

struct grug_ast tokens_to_ast(struct grug_tokens tokens) {
	(void)tokens;
	// TODO
	return (struct grug_ast){.helper_functions_count = 0, .on_functions_count = 0, .members_count = 0};
}

struct grug_tokens ast_to_tokens(struct grug_ast ast) {
	(void)ast;
	// TODO
	return (struct grug_tokens){.tokens_len = 0};
}

struct grug_string tokens_to_grug(struct grug_tokens tokens) {
	(void)tokens;
	// TODO
	return (struct grug_string){.len = 0};
}

struct grug_ast json_to_ast(struct grug_string json) {
	(void)json;
	// TODO
	return (struct grug_ast){.helper_functions_count = 0, .on_functions_count = 0, .members_count = 0};
}

struct grug_string ast_to_json(struct grug_ast ast) {
	(void)ast;
	// TODO
	return (struct grug_string){.len = 0};
}

struct grug_ast grug_to_ast(struct grug_string grug) {
	(void)grug;
	// TODO
	return (struct grug_ast){.helper_functions_count = 0, .on_functions_count = 0, .members_count = 0};
}

struct grug_string ast_to_grug(struct grug_ast ast) {
	(void)ast;
	// TODO
	return (struct grug_string){.len = 0};
}

struct grug_string grug_to_json(struct grug_string grug) {
	(void)grug;
	// TODO
	return (struct grug_string){.len = 0};
}

struct grug_string json_to_grug(struct grug_string json){ 
	(void)json;
	// TODO
	return (struct grug_string){.len = 0};
}
