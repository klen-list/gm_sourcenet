#include <filehandle.hpp>

namespace FileHandle
{

struct userdata
{
	FileHandle_t file;
	uint8_t type;
};

const uint8_t metaid = global::metabase + 6;
const char *metaname = "FileHandle_t";

void Push( lua_State *state, FileHandle_t file )
{
	if( file == nullptr )
	{
		LUA->PushNil( );
		return;
	}

	userdata *udata = static_cast<userdata *>( LUA->NewUserdata( sizeof( userdata ) ) );
	udata->type = metaid;
	udata->file = file;

	LUA->CreateMetaTableType( metaname, metaid );
	LUA->SetMetaTable( -2 );

	LUA->CreateTable( );
	lua_setfenv( state, -2 );
}

FileHandle_t Get( lua_State *state, int32_t index )
{
	global::CheckType( state, index, metaid, metaname );
	return static_cast<userdata *>( LUA->GetUserdata( index ) )->file;
}

LUA_FUNCTION_STATIC( eq )
{
	FileHandle_t handle1 = Get( state, 1 );
	FileHandle_t handle2 = Get( state, 2 );

	LUA->PushBool( handle1 == handle2 );

	return 1;
}

LUA_FUNCTION_STATIC( tostring )
{
	FileHandle_t ptr = Get( state, 1 );

	lua_pushfstring( state, global::tostring_format, metaname, ptr );

	return 1;
}

void Initialize( lua_State *state )
{
	LUA->CreateMetaTableType( metaname, metaid );

		LUA->PushCFunction( eq );
		LUA->SetField( -2, "__eq" );

		LUA->PushCFunction( tostring );
		LUA->SetField( -2, "__tostring" );

		LUA->PushCFunction( global::index );
		LUA->SetField( -2, "__index" );

		LUA->PushCFunction( global::newindex );
		LUA->SetField( -2, "__newindex" );

		LUA->PushCFunction( global::GetTable );
		LUA->SetField( -2, "GetTable" );

	LUA->Pop( 1 );
}

void Deinitialize( lua_State *state )
{
	LUA->PushNil( );
	LUA->SetField( -3, metaname );
}

}