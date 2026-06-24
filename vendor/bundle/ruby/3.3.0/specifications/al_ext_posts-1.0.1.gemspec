# -*- encoding: utf-8 -*-
# stub: al_ext_posts 1.0.1 ruby lib

Gem::Specification.new do |s|
  s.name = "al_ext_posts".freeze
  s.version = "1.0.1".freeze

  s.required_rubygems_version = Gem::Requirement.new(">= 0".freeze) if s.respond_to? :required_rubygems_version=
  s.metadata = { "allowed_push_host" => "https://rubygems.org", "homepage_uri" => "https://github.com/al-org-dev/al-ext-posts", "source_code_uri" => "https://github.com/al-org-dev/al-ext-posts" } if s.respond_to? :metadata=
  s.require_paths = ["lib".freeze]
  s.authors = ["al-org".freeze]
  s.date = "1980-01-02"
  s.description = "Jekyll plugin extracted from al-folio that imports external posts from RSS feeds or explicit URLs, with support for default tags and categories per source.".freeze
  s.email = ["dev@al-org.dev".freeze]
  s.homepage = "https://github.com/al-org-dev/al-ext-posts".freeze
  s.licenses = ["MIT".freeze]
  s.required_ruby_version = Gem::Requirement.new(">= 2.7".freeze)
  s.rubygems_version = "4.0.6".freeze
  s.summary = "Import external posts from RSS feeds and URLs".freeze

  s.installed_by_version = "3.6.7".freeze

  s.specification_version = 4

  s.add_runtime_dependency(%q<jekyll>.freeze, [">= 3.9".freeze, "< 5.0".freeze])
  s.add_runtime_dependency(%q<feedjira>.freeze, [">= 3.2".freeze, "< 5.0".freeze])
  s.add_runtime_dependency(%q<httparty>.freeze, [">= 0.18".freeze, "< 1.0".freeze])
  s.add_runtime_dependency(%q<nokogiri>.freeze, [">= 1.13".freeze, "< 2.0".freeze])
  s.add_development_dependency(%q<bundler>.freeze, [">= 2.0".freeze, "< 3.0".freeze])
  s.add_development_dependency(%q<rake>.freeze, ["~> 13.0".freeze])
end
