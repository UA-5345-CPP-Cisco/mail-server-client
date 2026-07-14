#pragma once

#include <functional>
#include <unordered_map>

#include "common.h"

class Router
{
public:
  using Handler = std::function<Response(Request const&)>;

  void add(
    http::verb method,
    std::string path,
    Handler handler
  )
  {
    m_routes.insert_or_assign(
      RouteKey{method, std::move(path)},
      std::move(handler));
  }

  void get(std::string path, Handler handler)
  {
    add(http::verb::get, std::move(path), std::move(handler));
  }

  void post(std::string path, Handler handler)
  {
    add(http::verb::post, std::move(path), std::move(handler));
  }

  [[nodiscard]] Response dispatch(Request const& request) const
  {
    const std::string path = extract_path(request.target());

    const auto iterator = m_routes.find(
      RouteKey{request.method(), path});

    if (iterator == m_routes.end())
    {
      return make_error(
        request,
        http::status::not_found,
        "Route not found");
    }

    try
    {
      return iterator->second(request);
    }
    catch (std::exception const& exception)
    {
      return make_error(
        request,
        http::status::internal_server_error,
        exception.what());
    }
  }

private:
  struct RouteKey
  {
    http::verb method;
    std::string path;

    bool operator==(const RouteKey& other) const
    {
      return method == other.method && path == other.path;
    }
  };

  struct RouteKeyHash
  {
    std::size_t operator()(RouteKey const& key) const
    {
      const auto method_hash =
        std::hash<unsigned>{}(
          static_cast<unsigned>(key.method));

      const auto path_hash =
        std::hash<std::string>{}(key.path);

      return method_hash ^
        (path_hash + 0x9e3779b9 +
          (method_hash << 6) +
          (method_hash >> 2));
    }
  };

  static std::string extract_path(beast::string_view target)
  {
    const auto query_position = target.find('?');

    return std::string{
      target.substr(0, query_position)
    };
  }

  std::unordered_map<RouteKey, Handler, RouteKeyHash> m_routes;
};
