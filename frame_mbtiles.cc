#include "wx/wxprec.h"
#include "wx/wx.h"
#include "sqlite3.h"
#include "sample.xpm"
#include <string>
#include <vector>

/////////////////////////////////////////////////////////////////////////////////////////////////////
//wxAppBitmap
/////////////////////////////////////////////////////////////////////////////////////////////////////

class wxAppBitmap : public wxApp
{
public:
  virtual bool OnInit();
};

wxIMPLEMENT_APP(wxAppBitmap);

/////////////////////////////////////////////////////////////////////////////////////////////////////
//wxFrameBitmap
/////////////////////////////////////////////////////////////////////////////////////////////////////

class wxFrameBitmap : public wxFrame
{
public:
  wxFrameBitmap(const wxString& title);
  ~wxFrameBitmap()
  {
    sqlite3_close(m_db);
  }
  void OnQuit(wxCommandEvent& event);
  void OnAbout(wxCommandEvent& event);
  void OnPaint(wxPaintEvent& event);
  wxBitmap m_bitmap;

  int get_tiles(size_t zoom_level, size_t  tile_column, size_t tile_row);
  int get_tables(const std::string &file_name, std::vector<std::string> &tables);
  int get_metadata();

private:
  sqlite3 *m_db;

private:
  wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(wxFrameBitmap, wxFrame)
EVT_MENU(wxID_EXIT, wxFrameBitmap::OnQuit)
EVT_MENU(wxID_ABOUT, wxFrameBitmap::OnAbout)
EVT_PAINT(wxFrameBitmap::OnPaint)
wxEND_EVENT_TABLE()

/////////////////////////////////////////////////////////////////////////////////////////////////////
//wxAppBitmap::OnInit()
/////////////////////////////////////////////////////////////////////////////////////////////////////

bool wxAppBitmap::OnInit()
{
  if (!wxApp::OnInit())
  {
    return false;
  }
  wxFrameBitmap *frame = new wxFrameBitmap("MBTiles");
  frame->Show(true);
  frame->Maximize();
  return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//wxFrameBitmap::wxFrameBitmap
/////////////////////////////////////////////////////////////////////////////////////////////////////

wxFrameBitmap::wxFrameBitmap(const wxString& title)
  : wxFrame(NULL, wxID_ANY, title)
{
  SetIcon(wxICON(sample));
  wxMenu *menu_file = new wxMenu;
  menu_file->Append(wxID_EXIT, "E&xit\tAlt-X", "Quit this program");
  wxMenu *menu_help = new wxMenu;
  menu_help->Append(wxID_ABOUT, "&About\tF1", "Show about dialog");
  wxMenuBar *menu_bar = new wxMenuBar();
  menu_bar->Append(menu_file, "&File");
  menu_bar->Append(menu_help, "&Help");
  SetMenuBar(menu_bar);
  CreateStatusBar(2);
  SetStatusText("Ready");

  wxInitAllImageHandlers();

  std::vector<std::string> tables;
  if (get_tables("countries-raster.mbtiles", tables) < 0)
  {

  }

  //Metadata
  //The database MUST contain a table or view named `metadata`.
  //This table or view MUST yield exactly two columns of type `text`, named `name` and
  //`value`. A typical create statement for the `metadata` table:
  //CREATE TABLE metadata(name text, value text);

  //Tiles
  //The database MUST contain a table named `tiles`.
  //The table MUST contain three columns of type `integer`, named `zoom_level`, `tile_column`,
  //`tile_row`, and one of type `blob`, named `tile_data`.
  //A typical `create` statement for the `tiles` table:
  //CREATE TABLE tiles(zoom_level integer, tile_column integer, tile_row integer, tile_data blob);

  for (size_t idx = 0; idx < tables.size(); idx++)
  {
    wxLogDebug("%s", tables.at(idx));
  }

  if (get_metadata() < 0)
  {

  }

  if (get_tiles(0, 0, 0) < 0)
  {

  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//wxFrameBitmap::OnQuit
/////////////////////////////////////////////////////////////////////////////////////////////////////

void wxFrameBitmap::OnQuit(wxCommandEvent& WXUNUSED(event))
{
  Close(true);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//wxFrameBitmap::OnAbout
/////////////////////////////////////////////////////////////////////////////////////////////////////

void wxFrameBitmap::OnAbout(wxCommandEvent& WXUNUSED(event))
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//wxFrameBitmap::OnAbout
/////////////////////////////////////////////////////////////////////////////////////////////////////

void wxFrameBitmap::OnPaint(wxPaintEvent& WXUNUSED(event))
{
  wxPaintDC dc(this);
  dc.DrawBitmap(m_bitmap, 0, 0, true);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
//wxFrameBitmap::get_tables
/////////////////////////////////////////////////////////////////////////////////////////////////////

int wxFrameBitmap::get_tables(const std::string &file_name, std::vector<std::string> &tables)
{
  sqlite3_stmt *stmt = 0;

  if (sqlite3_open_v2(file_name.c_str(), &m_db, SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK)
  {
    sqlite3_errmsg(m_db);
    sqlite3_close(m_db);
    return SQLITE_ERROR;
  }

  if (sqlite3_prepare_v2(m_db, "SELECT name FROM sqlite_master WHERE type='table';", -1, &stmt, 0) != SQLITE_OK)
  {
    sqlite3_errmsg(m_db);
    sqlite3_close(m_db);
    return SQLITE_ERROR;
  }

  while (sqlite3_step(stmt) == SQLITE_ROW)
  {
    std::string table = (const char*)sqlite3_column_text(stmt, 0);
    tables.push_back(table);
  }

  sqlite3_finalize(stmt);
  return SQLITE_OK;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//wxFrameBitmap::get_metadata
/////////////////////////////////////////////////////////////////////////////////////////////////////

int wxFrameBitmap::get_metadata()
{
  sqlite3_stmt *stmt = 0;

  if (sqlite3_prepare_v2(m_db, "SELECT * FROM metadata;", -1, &stmt, 0) != SQLITE_OK)
  {
    sqlite3_errmsg(m_db);
    sqlite3_close(m_db);
    return SQLITE_ERROR;
  }

  while (sqlite3_step(stmt) == SQLITE_ROW)
  {
    std::string row_name = (const char*)sqlite3_column_text(stmt, 0); //field 'name '
    std::string row_value = (const char*)sqlite3_column_text(stmt, 1); //field 'value '
    wxLogDebug("%s | %s", row_name, row_value);
  }

  sqlite3_finalize(stmt);
  return SQLITE_OK;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
//wxFrameBitmap::get_tile
/////////////////////////////////////////////////////////////////////////////////////////////////////

int wxFrameBitmap::get_tiles(size_t zoom_level, size_t  tile_column, size_t tile_row)
{
  sqlite3_stmt *stmt = 0;
  std::string sql = "SELECT tile_data FROM tiles WHERE ";
  sql += "zoom_level=";
  sql += std::to_string(zoom_level);
  sql += " AND tile_column=";
  sql += std::to_string(tile_column);
  sql += " AND tile_row=";
  sql += std::to_string(tile_row);
  sql += ";";

  if (sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, 0) != SQLITE_OK)
  {
    sqlite3_errmsg(m_db);
    sqlite3_close(m_db);
    return SQLITE_ERROR;
  }

  while (sqlite3_step(stmt) == SQLITE_ROW)
  {
    FILE *fp;
    int col = 0;
    char name[50];
    snprintf(name, 20, "tile.%zd.%zd.%zd.png", zoom_level, tile_column, tile_row);
    if ((fp = fopen(name, "wb")) == NULL)
    {
    }
    int size_blob = sqlite3_column_bytes(stmt, col);
    const void *blob = sqlite3_column_blob(stmt, col);
    fwrite(blob, size_blob, 1, fp);
    fclose(fp);
    m_bitmap = wxBitmap::NewFromPNGData(blob, size_blob);
    const int w = m_bitmap.GetWidth();
    const int h = m_bitmap.GetHeight();
  }

  sqlite3_finalize(stmt);
  return SQLITE_OK;
}
