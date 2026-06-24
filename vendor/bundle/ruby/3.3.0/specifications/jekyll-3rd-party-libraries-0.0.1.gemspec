# -*- encoding: utf-8 -*-
# stub: jekyll-3rd-party-libraries 0.0.1 ruby lib

Gem::Specification.new do |s|
  s.name = "jekyll-3rd-party-libraries".freeze
  s.version = "0.0.1".freeze

  s.required_rubygems_version = Gem::Requirement.new(">= 0".freeze) if s.respond_to? :required_rubygems_version=
  s.metadata = { "bug_tracker_uri" => "https://github.com/george-gca/jekyll-3rd-party-libraries/issues", "changelog_uri" => "https://github.com/george-gca/jekyll-3rd-party-libraries/releases", "source_code_uri" => "https://github.com/george-gca/jekyll-3rd-party-libraries" } if s.respond_to? :metadata=
  s.require_paths = ["lib".freeze]
  s.authors = ["George Corr\u00EAa de Ara\u00FAjo".freeze]
  s.date = "2025-01-23"
  s.description = "Force updating cached files and resources in a Jekyll site by adding a hash.".freeze
  s.homepage = "https://github.com/george-gca/jekyll-3rd-party-libraries".freeze
  s.licenses = ["MIT".freeze]
  s.required_ruby_version = Gem::Requirement.new(">= 2.3.0".freeze)
  s.rubygems_version = "3.6.3".freeze
  s.summary = "Force updating cached files and resources in a Jekyll site.".freeze

  s.installed_by_version = "3.6.7".freeze

  s.specification_version = 4

  s.add_runtime_dependency(%q<jekyll>.freeze, [">= 3.6".freeze, "< 5.0".freeze])
  s.add_runtime_dependency(%q<css_parser>.freeze, [">= 1.6".freeze, "< 2.0".freeze])
  s.add_runtime_dependency(%q<nokogiri>.freeze, [">= 1.8".freeze, "< 2.0".freeze])
end
